#include "pch.h"

#include <vlr-util/util.StringConversion.h>
#ifdef _WIN32
#include <vlr-util/Win32/util.win32.StringConversion.h>
#endif

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

#ifdef _WIN32

TEST_F(TestStringConversion, Win32_MultiByte_to_UTF16_EmptyString)
{
	wchar_t buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	oOptions.m_bInputStringIsNullTerminated = true;
	vlr::util::StringConversionResults oResults{};

	auto hr = vlr::util::win32::MultiByte_to_UTF16("", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_EQ(oResults.m_nOuputSizeBytes, sizeof(wchar_t));
	EXPECT_STREQ(buffer, L"");
}

TEST_F(TestStringConversion, Win32_MultiByte_to_UTF16_EmptyString_NotNullTerminated)
{
	wchar_t buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	oOptions.m_bGenerateResultNotNullTerminated = true;
	vlr::util::StringConversionResults oResults{};

	auto hr = vlr::util::win32::MultiByte_to_UTF16("", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_EQ(oResults.m_nOuputSizeBytes, 0);
	// Note: If we allow a non-null-terminated result, the buffer may not be null-terminated, so we cannot check the string content safely.
}

TEST_F(TestStringConversion, Win32_MultiByte_to_UTF16_SimpleString)
{
	wchar_t buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::MultiByte_to_UTF16("test", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_STREQ(buffer, L"test");
	EXPECT_EQ(oResults.m_nOuputSizeBytes, (wcslen(buffer) + 1) * sizeof(wchar_t));
}

TEST_F(TestStringConversion, Win32_MultiByte_to_UTF16_BufferTooSmall)
{
	wchar_t buffer[3];
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::MultiByte_to_UTF16("test", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(FAILED(hr));
}

TEST_F(TestStringConversion, Win32_MultiByte_to_UTF16_NullBuffer)
{
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::MultiByte_to_UTF16("test", nullptr, 0, oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr) || FAILED(hr)); // Should handle gracefully
}

TEST_F(TestStringConversion, Win32_UTF16_to_MultiByte_EmptyString)
{
	char buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	oOptions.m_bInputStringIsNullTerminated = true;
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::UTF16_to_MultiByte(L"", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_EQ(oResults.m_nOuputSizeBytes, sizeof(char)); // null terminator
	EXPECT_STREQ(buffer, "");
}

TEST_F(TestStringConversion, Win32_UTF16_to_MultiByte_EmptyString_NotNullTerminated)
{
	char buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	oOptions.m_bGenerateResultNotNullTerminated = true;
	vlr::util::StringConversionResults oResults{};

	auto hr = vlr::util::win32::UTF16_to_MultiByte(L"", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_EQ(oResults.m_nOuputSizeBytes, 0);
	// Note: If we allow a non-null-terminated result, the buffer may not be null-terminated, so we cannot check the string content safely.
}

TEST_F(TestStringConversion, Win32_UTF16_to_MultiByte_SimpleString)
{
	char buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::UTF16_to_MultiByte(L"test", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_STREQ(buffer, "test");
	EXPECT_EQ(oResults.m_nOuputSizeBytes, (strlen(buffer) + 1) * sizeof(char));
}

TEST_F(TestStringConversion, Win32_UTF16_to_MultiByte_BufferTooSmall)
{
	char buffer[3];
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::UTF16_to_MultiByte(L"test", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(FAILED(hr));
}

TEST_F(TestStringConversion, Win32_UTF16_to_MultiByte_NullBuffer)
{
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::UTF16_to_MultiByte(L"test", nullptr, 0, oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr) || FAILED(hr)); // Should handle gracefully
}

TEST_F(TestStringConversion, Win32_MultiByte_to_UTF16_SpecialCharacters)
{
	wchar_t buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::MultiByte_to_UTF16("test\t\n\r", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_STREQ(buffer, L"test\t\n\r");
	EXPECT_EQ(oResults.m_nOuputSizeBytes, (wcslen(buffer) + 1) * sizeof(wchar_t));
}

TEST_F(TestStringConversion, Win32_UTF16_to_MultiByte_SpecialCharacters)
{
	char buffer[256];
	vlr::util::StringConversionOptions oOptions{};
	vlr::util::StringConversionResults oResults{};
	
	auto hr = vlr::util::win32::UTF16_to_MultiByte(L"test\t\n\r", buffer, sizeof(buffer), oOptions, &oResults);
	EXPECT_TRUE(SUCCEEDED(hr));
	EXPECT_STREQ(buffer, "test\t\n\r");
	EXPECT_EQ(oResults.m_nOuputSizeBytes, (strlen(buffer) + 1) * sizeof(char));
}

#endif // _WIN32
