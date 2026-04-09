#include "pch.h"

#include <vlr-util/include.fmt.h>

#include <vlr-util/util.convert.StringConversion.h>
#include <vlr-util/zstring_view.h>
#include <vlr-util/StringCompare.h>
#include <vlr-util/ActionOnDestruction.h>

using namespace vlr::util;
using namespace vlr::util::Convert;

TEST(StringConversion, ToStdStringA_Nullptr_And_Pointer)
{
	// nullptr_t overload
	std::string s1 = ToStdStringA(nullptr);
	EXPECT_EQ(s1, "");

	// const char* null
	const char* psaNull = nullptr;
	std::string s2 = ToStdStringA(psaNull);
	EXPECT_EQ(s2, "");

	// const wchar_t* null (cross-width)
	const wchar_t* pswNull = nullptr;
	std::string s3 = ToStdStringA(pswNull);
	EXPECT_EQ(s3, "");

	// const char* non-null
	const char* pStr = "abc";
	std::string s4 = ToStdStringA(pStr);
	EXPECT_EQ(s4, "abc");

	// const wchar_t* non-null (cross-width)
	const wchar_t* pwStr = L"def";
	std::string s5 = ToStdStringA(pwStr);
	EXPECT_EQ(s5, ToStdStringA(std::wstring_view{ pwStr }));
}

TEST(StringConversion, ToStdStringW_Nullptr_And_Pointer)
{
	// nullptr_t overload
	std::wstring s1 = ToStdStringW(nullptr);
	EXPECT_EQ(s1, L"");

	// const char* null (cross-width)
	const char* psaNull = nullptr;
	std::wstring s2 = ToStdStringW(psaNull);
	EXPECT_EQ(s2, L"");

	// const wchar_t* null
	const wchar_t* pswNull = nullptr;
	std::wstring s3 = ToStdStringW(pswNull);
	EXPECT_EQ(s3, L"");

	// const wchar_t* non-null
	const wchar_t* pStr = L"xyz";
	std::wstring s4 = ToStdStringW(pStr);
	EXPECT_EQ(s4, L"xyz");

	// const char* non-null (cross-width)
	const char* pcStr = "uvw";
	std::wstring s5 = ToStdStringW(pcStr);
	EXPECT_EQ(s5, ToStdStringW(std::string_view{ pcStr }));
}

TEST(StringConversion, ToStdStringA_OverloadResolution)
{
	// Should resolve to std::string_view overload
	std::string_view sv = "hello";
	std::string s1 = ToStdStringA(sv);
	EXPECT_EQ(s1, "hello");

	// Should resolve to std::string overload
	std::string s2 = ToStdStringA(std::string("world"));
	EXPECT_EQ(s2, "world");

	// Should resolve to const char* overload
	const char* cstr = "test";
	std::string s3 = ToStdStringA(cstr);
	EXPECT_EQ(s3, "test");

	// Should resolve to nullptr_t overload
	std::string s4 = ToStdStringA(nullptr);
	EXPECT_EQ(s4, "");
}

TEST(StringConversion, ToStdStringW_OverloadResolution)
{
	// Should resolve to std::wstring_view overload
	std::wstring_view sv = L"wide";
	std::wstring s1 = ToStdStringW(sv);
	EXPECT_EQ(s1, L"wide");

	// Should resolve to std::wstring overload
	std::wstring s2 = ToStdStringW(std::wstring(L"string"));
	EXPECT_EQ(s2, L"string");

	// Should resolve to const wchar_t* overload
	const wchar_t* wcstr = L"testw";
	std::wstring s3 = ToStdStringW(wcstr);
	EXPECT_EQ(s3, L"testw");

	// Should resolve to nullptr_t overload
	std::wstring s4 = ToStdStringW(nullptr);
	EXPECT_EQ(s4, L"");
}

// Optionally, test that ambiguous calls do not compile (static_asserts or SFINAE)
// but this is not possible in a runtime test; compilation will fail if ambiguous.

struct Test_util_convert_StringConversion
	: public testing::Test
{
	vlr::StringCompare::CComparator m_oStringCompare = vlr::StringCompare::CS();

	static constexpr decltype(auto) GetTestStringA()
	{
		return "TestString";
	}
	static constexpr decltype(auto) GetTestStringW()
	{
		return L"TestString";
	}
	static constexpr decltype(auto) GetTestStringT()
	{
		return _T("TestString");
	}

	static auto GetTestString_std_string()
	{
		return std::string{ GetTestStringA() };
	}
	static auto GetTestString_std_wstring()
	{
		return std::wstring{ GetTestStringW() };
	}
	static constexpr auto GetTestString_std_string_view()
	{
		return std::string_view{ GetTestStringA() };
	}
	static constexpr auto GetTestString_std_wstring_view()
	{
		return std::wstring_view{ GetTestStringW() };
	}
	static constexpr auto GetTestString_vlr_zstring_view()
	{
		return vlr::zstring_view{ GetTestStringA() };
	}
	static constexpr auto GetTestString_vlr_wzstring_view()
	{
		return vlr::wzstring_view{ GetTestStringW() };
	}
	static auto GetTestString_ATL_CStringA()
	{
		return ATL::CStringA{ GetTestStringA() };
	}
	static auto GetTestString_ATL_CStringW()
	{
		return ATL::CStringW{ GetTestStringW() };
	}
	static auto GetTestString_bstr_t()
	{
		return _bstr_t{ GetTestStringW() };
	}
	static auto GetTestString_ATL_CComBSTR()
	{
		return ATL::CComBSTR{ GetTestStringW() };
	}

	template <typename TString>
	void ValidateAllToStringConversions(const TString& sValue)
	{
		// to std::string

		auto tString_asStdStringA = Convert::ToStdStringA(sValue);
		EXPECT_STREQ(tString_asStdStringA.c_str(), GetTestStringA());
		auto tString_asStdStringW = Convert::ToStdStringW(sValue);
		EXPECT_STREQ(tString_asStdStringW.c_str(), GetTestStringW());
		auto tString_asStdStringT = Convert::ToStdString(sValue);
		EXPECT_STREQ(tString_asStdStringT.c_str(), GetTestStringT());

		// to CString

		auto tString_asCStringA = Convert::ToCStringA(sValue);
		EXPECT_STREQ(tString_asCStringA, GetTestStringA());
		auto tString_asCStringW = Convert::ToCStringW(sValue);
		EXPECT_STREQ(tString_asCStringW, GetTestStringW());
		auto tString_asCStringT = Convert::ToCString(sValue);
		EXPECT_STREQ(tString_asCStringT, GetTestStringT());

		// to _bstr_t

		auto tString_as_bstr_t = Convert::To_bstr_t(sValue);
		EXPECT_STREQ(static_cast<LPCWSTR>(tString_as_bstr_t), GetTestStringW());
	}

	template <typename TString>
	void ValidateAllToFmtArgConversions(const TString& sValue)
	{
		// to String

		auto tFmtArg_asStdStringA = Convert::ToFmtArg_StringA(sValue);
		EXPECT_TRUE(m_oStringCompare.AreEqual(tFmtArg_asStdStringA, GetTestStringA()));
		auto tFmtArg_asStdStringW = Convert::ToFmtArg_StringW(sValue);
		EXPECT_TRUE(m_oStringCompare.AreEqual(tFmtArg_asStdStringW, GetTestStringW()));
		auto tFmtArg_asStdStringT = Convert::ToFmtArg_String(sValue);
		EXPECT_TRUE(m_oStringCompare.AreEqual(tFmtArg_asStdStringT, GetTestStringT()));
	};
};

TEST_F(Test_util_convert_StringConversion, ToString)
{
	ValidateAllToStringConversions(GetTestString_std_string());
	ValidateAllToStringConversions(GetTestString_std_wstring());
	ValidateAllToStringConversions(GetTestString_std_string_view());
	ValidateAllToStringConversions(GetTestString_std_wstring_view());
	ValidateAllToStringConversions(GetTestString_vlr_zstring_view());
	ValidateAllToStringConversions(GetTestString_vlr_wzstring_view());
	ValidateAllToStringConversions(GetTestString_ATL_CStringA());
	ValidateAllToStringConversions(GetTestString_ATL_CStringW());
	ValidateAllToStringConversions(GetTestString_bstr_t());
	ValidateAllToStringConversions(GetTestString_ATL_CComBSTR());
}

TEST(StringConversion, NonASCII)
{
	using namespace vlr::util;

	std::string saTest = "caractères ASCII";

	auto swValue = Convert::ToStdStringW_FromSystemDefaultASCII(saTest);
	//std::wcout << L"Test string (UTF16): " << swValue << std::endl;
	//for (auto cChar : swValue)
	//{
	//	std::wcout << std::hex << (unsigned short)cChar << L" ";
	//}
	//std::wcout << std::endl;

	// Note: Valid UTF16 encoding obtained (manually) via online resource
	// (https://www.browserling.com/tools/utf16-encode)
	std::vector<unsigned short> vecExpectedValue{ 0x63, 0x61, 0x72, 0x61, 0x63, 0x74, 0xe8, 0x72, 0x65, 0x73, 0x20, 0x41, 0x53, 0x43, 0x49, 0x49 };

	{
		size_t nExpectedValueIndex = 0;
		for (auto cChar : swValue)
		{
			ASSERT_LT(nExpectedValueIndex, vecExpectedValue.size());
			EXPECT_EQ(cChar, vecExpectedValue[nExpectedValueIndex++]);
		}
	}
}

struct StringConversionUnicode
	: public testing::Test
{
	std::locale m_oLocale_UTF8 = std::locale{ "en_US.UTF-8" };

	static constexpr vlr::wzstring_view svwzTestValue_UTF16_1 = L"wyj\u015Bcie";

	std::string GetByteDisplay(const std::string& saValue)
	{
		std::string saByteDisplay;
		saByteDisplay.reserve(saValue.length() * 3 + 1);

		auto pBuffer = reinterpret_cast<const BYTE*>(saValue.data());
		for (size_t nIndex = 0; nIndex < saValue.length(); ++nIndex)
		{
			saByteDisplay += fmt::format("{:02X} ", pBuffer[nIndex]);
		}

		return saByteDisplay;
	}

	std::string GetByteDisplay(const std::wstring& swValue)
	{
		std::string saByteDisplay;
		saByteDisplay.reserve(swValue.length() * 3 + 1);

		auto pBuffer = reinterpret_cast<const BYTE*>(swValue.data());
		for (size_t nIndex = 0; nIndex < swValue.length(); ++nIndex)
		{
			saByteDisplay += fmt::format("{:02X} ", pBuffer[nIndex]);
		}

		return saByteDisplay;
	}

	void TestRoundTrip_FromUTF8(const std::string& saValue)
	{
		fmt::print("Test value (UTF-8): {}\n", saValue);
		//std::cout << "Test value (UTF-8): " << saValue << std::endl;
		fmt::print("Bytes: {}\n", GetByteDisplay(saValue));

		auto swValue = vlr::util::Convert::ToStdStringW(saValue);
		fmt::print(L"Test value (UTF-16): {}\n", swValue);
		fmt::print("Bytes: {}\n", GetByteDisplay(swValue));

		auto saValue_Copy = vlr::util::Convert::ToStdStringA(swValue);
		fmt::print("Test value (UTF-8, copy): {}\n", saValue_Copy);
		fmt::print("Bytes: {}\n", GetByteDisplay(saValue_Copy));

		EXPECT_EQ(saValue, saValue_Copy);
	}

	void TestRoundTrip_FromUTF16(const std::wstring& swValue)
	{
		fmt::print(L"Test value (UTF-16): {}\n", swValue);
		fmt::print("Bytes: {}\n", GetByteDisplay(swValue));

		auto saValue_Copy = vlr::util::Convert::ToStdStringA(swValue);
		fmt::print("Test value (UTF-8, copy): {}\n", saValue_Copy);
		fmt::print("Bytes: {}\n", GetByteDisplay(saValue_Copy));

		auto swValue_Copy = vlr::util::Convert::ToStdStringW(saValue_Copy);
		fmt::print(L"Test value (UTF-16, copy): {}\n", swValue_Copy);
		fmt::print("Bytes: {}\n", GetByteDisplay(saValue_Copy));

		EXPECT_EQ(swValue, swValue_Copy);
	}
};

TEST_F(StringConversionUnicode, TestRoundTrip)
{
	std::locale::global(m_oLocale_UTF8);

	TestRoundTrip_FromUTF16(svwzTestValue_UTF16_1.toStdString());
}

TEST_F(Test_util_convert_StringConversion, ToFmtArg_String)
{
	ValidateAllToFmtArgConversions(GetTestString_std_string());
	ValidateAllToFmtArgConversions(GetTestString_std_wstring());
	ValidateAllToFmtArgConversions(GetTestString_std_string_view());
	ValidateAllToFmtArgConversions(GetTestString_std_wstring_view());
	ValidateAllToFmtArgConversions(GetTestString_vlr_zstring_view());
	ValidateAllToFmtArgConversions(GetTestString_vlr_wzstring_view());
	ValidateAllToFmtArgConversions(GetTestString_ATL_CStringA());
	ValidateAllToFmtArgConversions(GetTestString_ATL_CStringW());
	ValidateAllToFmtArgConversions(GetTestString_bstr_t());
	ValidateAllToFmtArgConversions(GetTestString_ATL_CComBSTR());

	// Check that constexpr conversions work as/where expected

	{
		constexpr auto svFmtArg_StringA = Convert::ToFmtArg_StringA(GetTestStringA());
	}
	{
		constexpr auto svFmtArg_StringW = Convert::ToFmtArg_StringW(GetTestStringW());
	}
	{
		constexpr auto svFmtArg_String = Convert::ToFmtArg_String(GetTestStringT());
	}
	// This will not work: the std::string variables are not constexpr, and cannot be so before C++20
	//{
	//	constexpr auto svFmtArg_StringA = Convert::ToFmtArg_StringA(straTest);
	//}
	//{
	//	constexpr auto svFmtArg_StringW = Convert::ToFmtArg_StringW(strwTest);
	//}
	// These need to convert constexpr variables...
	{
		static constexpr auto svaTestVar = std::string_view{ GetTestStringA() };
		constexpr auto svFmtArg_StringA = Convert::ToFmtArg_StringA(svaTestVar);
	}
	{
		static constexpr auto svwTestVar = std::wstring_view{ GetTestStringW() };
		constexpr auto svFmtArg_StringW = Convert::ToFmtArg_StringW(svwTestVar);
	}
	// These need to convert constexpr variables...
	{
		static constexpr auto svaTestVar = vlr::zstring_view{ GetTestStringA() };
		constexpr auto svFmtArg_StringA = Convert::ToFmtArg_StringA(svaTestVar);
	}
	{
		static constexpr auto svwTestVar = vlr::wzstring_view{ GetTestStringW() };
		constexpr auto svFmtArg_StringW = Convert::ToFmtArg_StringW(svwTestVar);
	}
	// Not constexpr compatible: CString, _bstr_t, CComBSTR
}

// Note: This test helps ensure that we correctly handle the case of a BSTR (with a prefixed length) which is not otherwise NULL-terminated.

TEST(util_convert_StringConversion, LengthPrefixedBSTR)
{
	std::vector<BYTE> vecFakeBstrBuffer;
	// Size is 4-byte length prefix, plus string data
	// Adding a BYTE of "bad" data, which is non-NULL, to ensure we don't read it
	constexpr auto pcwszTestValue = L"Test";
	constexpr size_t nTestValueLengthBytes = sizeof(pcwszTestValue) * sizeof(pcwszTestValue[0]);
	constexpr auto pcwszInvalidSuffix = L"FF";
	constexpr size_t nInvalidSuffixLengthBytes = sizeof(pcwszInvalidSuffix) * sizeof(pcwszInvalidSuffix[0]);
	vecFakeBstrBuffer.resize(4 + nTestValueLengthBytes + nInvalidSuffixLengthBytes);

	auto pFakeBstrBuffer = vecFakeBstrBuffer.data();
	*(reinterpret_cast<DWORD*>(pFakeBstrBuffer)) = nTestValueLengthBytes;
	memcpy_s(pFakeBstrBuffer + 4, vecFakeBstrBuffer.size() - 4, pcwszTestValue, nTestValueLengthBytes);
	memcpy_s(pFakeBstrBuffer + 4 + nTestValueLengthBytes, vecFakeBstrBuffer.size() - 4 - nTestValueLengthBytes, pcwszInvalidSuffix, nInvalidSuffixLengthBytes);
	BSTR bsFakeBstrPtr = reinterpret_cast<BSTR>(pFakeBstrBuffer + 4);

	CComBSTR bsFakeBstr;
	bsFakeBstr.AssignBSTR(bsFakeBstrPtr);
	auto oOnDestroy_Detach = vlr::MakeActionOnDestruction([&]() {
		bsFakeBstr.Detach();
	});

	auto tString_asStdStringW = Convert::ToStdStringW(bsFakeBstr);
	EXPECT_EQ(tString_asStdStringW.length(), sizeof(pcwszTestValue));
	EXPECT_STREQ(tString_asStdStringW.c_str(), pcwszTestValue);
}

TEST(util_convert_StringConversion, empty_CComBSTR)
{
	auto bsValue = CComBSTR{};

	auto tString_asStdStringW = Convert::ToStdStringW(bsValue);
	EXPECT_EQ(tString_asStdStringW.length(), 0);
	EXPECT_STREQ(tString_asStdStringW.c_str(), L"");
}
