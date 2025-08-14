#include "pch.h"

#include <vlr-util/util.StringConversion.h>

using namespace vlr::util;

struct CStringConversionForTesting
	: public CStringConversion
{
	using TBaseClass = CStringConversion;

	inline HRESULT MultiByte_to_UTF16_choice(
		vlr::util::choice<0>&&,
		std::string_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults)
	{
		return TBaseClass::MultiByte_to_UTF16_choice(
			vlr::util::choice<0>{},
			svValue,
			pOutputBuffer,
			nOutputBufferLengthBytes,
			oStringConversionOptions,
			pStringConversionResults);
	}

};

struct StringConversionDataStore
{
	static auto& GetSharedInstanceMutable()
	{
		static auto theInstance = StringConversionDataStore{};
		return theInstance;
	}

	std::string m_saConversionSource;
	std::wstring m_swConversionSource;
};

struct TestStringConversion
	: public testing::Test
{
	CStringConversionForTesting m_oStringConversion;

	TestStringConversion()
	{
		auto& oExternalImpl = vlr::util::StringConversion::CExternalImpl::GetSharedInstanceMutable();

		oExternalImpl.Set_MultiByte_to_UTF16_StdString_Inline([](std::string_view svValue, const StringConversionOptions& oStringConversionOptions)
		{
			auto& oStringConversionDataStore = StringConversionDataStore::GetSharedInstanceMutable();
			oStringConversionDataStore.m_saConversionSource = svValue;

			return L"";
		});
		oExternalImpl.Set_UTF16_to_MultiByte_StdString_Inline([](std::wstring_view svValue, const StringConversionOptions& oStringConversionOptions)
		{
			auto& oStringConversionDataStore = StringConversionDataStore::GetSharedInstanceMutable();
			oStringConversionDataStore.m_swConversionSource = svValue;

			return "";
		});
	}
	~TestStringConversion()
	{
		auto& oExternalImpl = vlr::util::StringConversion::CExternalImpl::GetSharedInstanceMutable();

		oExternalImpl.Set_MultiByte_to_UTF16_StdString_Inline({});
		oExternalImpl.Set_UTF16_to_MultiByte_StdString_Inline({});
	}
};

TEST_F(TestStringConversion, MultiByte_to_UTF16_choice_0)
{
	auto& oStringConversionDataStore = StringConversionDataStore::GetSharedInstanceMutable();

	auto swValue = m_oStringConversion.Inline_MultiByte_to_UTF16_StdString(std::string_view{ "test" });
	EXPECT_STREQ(oStringConversionDataStore.m_saConversionSource.c_str(), "test");
}

TEST_F(TestStringConversion, UTF16_to_MultiByte_choice_0)
{
	auto& oStringConversionDataStore = StringConversionDataStore::GetSharedInstanceMutable();

	auto swValue = m_oStringConversion.Inline_UTF16_to_MultiByte_StdString(std::wstring_view{ L"test" });
	EXPECT_STREQ(oStringConversionDataStore.m_swConversionSource.c_str(), L"test");
}

TEST_F(TestStringConversion, EmptyStringComversion_UTF16_to_MB)
{
	static const std::wstring swTest = L"";

	auto saValue = m_oStringConversion.Inline_UTF16_to_MultiByte_StdString(swTest);
	EXPECT_TRUE(saValue.empty());
	EXPECT_STREQ(saValue.c_str(), "");
}

TEST_F(TestStringConversion, EmptyStringComversion_MB_to_UTF16)
{
	static const std::string saTest = "";

	auto swValue = m_oStringConversion.Inline_MultiByte_to_UTF16_StdString(saTest);
	EXPECT_TRUE(swValue.empty());
	EXPECT_STREQ(swValue.c_str(), L"");
}

TEST_F(TestStringConversion, InlineConversionsAreNoexcept)
{
	static_assert(noexcept(m_oStringConversion.Inline_MultiByte_to_UTF16_StdString(std::string_view{})));
	static_assert(noexcept(m_oStringConversion.Inline_UTF16_to_MultiByte_StdString(std::wstring_view{})));
	static_assert(noexcept(m_oStringConversion.Inline_MultiByte_to_UTF16_StdString(std::string{})));
	static_assert(noexcept(m_oStringConversion.Inline_UTF16_to_MultiByte_StdString(std::wstring{})));
}
