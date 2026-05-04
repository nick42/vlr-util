#include "pch.h"
#include "zstring_view.h"

#include <gtest/gtest.h>

#include "vlr-util/zstring_view.h"

static constexpr auto pcszTestStringA = "test";
static constexpr auto pcszTestStringW = L"test";
static constexpr auto pcszTestStringT = _T("test");

TEST(zstring_view, char_base)
{
	static constexpr auto pcszTestString = pcszTestStringA;
	auto nTestStringLen = strlen(pcszTestString);
	auto svzTest = vlr::zstring_view{ pcszTestString };

	EXPECT_EQ(svzTest.length(), nTestStringLen);
	EXPECT_EQ(strcmp(svzTest, pcszTestString), 0);
	// Assure svz is actually NULL-terminated
	auto pcszTest = static_cast<const char*>(svzTest);
	EXPECT_EQ(pcszTest[nTestStringLen], '\0');
}

TEST(zstring_view, wchar_base)
{
	static constexpr auto pcszTestString = pcszTestStringW;
	auto nTestStringLen = wcslen(pcszTestString);
	auto svzTest = vlr::wzstring_view{ pcszTestString };

	EXPECT_EQ(svzTest.length(), wcslen(pcszTestString));
	EXPECT_EQ(wcscmp(svzTest, pcszTestString), 0);
	// Assure svz is actually NULL-terminated
	auto pcszTest = static_cast<const wchar_t*>(svzTest);
	EXPECT_EQ(pcszTest[nTestStringLen], L'\0');
}

TEST(zstring_view, constexpr_base)
{
	static constexpr auto pcszTestString = pcszTestStringT;
	auto nTestStringLen = _tcslen(pcszTestString);
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	EXPECT_EQ(svzTest.length(), _tcslen(pcszTestString));
	EXPECT_EQ(_tcscmp(svzTest, pcszTestString), 0);
	// Assure svz is actually NULL-terminated
	auto pcszTest = static_cast<const TCHAR*>(svzTest);
	EXPECT_EQ(pcszTest[nTestStringLen], _T('\0'));
}

// zstring_view should be functionally identical to std::string_view, in terms of its semantics and methods

TEST(zstring_view, inherited_behavior)
{
	static constexpr auto pcszTestString = pcszTestStringT;
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svText = vlr::tstring_view{ pcszTestString };

	EXPECT_EQ(svzTest.data(), svText.data());
	EXPECT_EQ(svzTest.length(), svText.length());
}

// Corner case tests for zstring_view
TEST(zstring_view, empty_string)
{
	static constexpr auto pcszTestString = _T("");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	EXPECT_EQ(svzTest.length(), 0);
	EXPECT_EQ(svzTest.data()[0], _T('\0'));
}

TEST(zstring_view, trailing_end)
{
	static constexpr auto pcszTestString = _T("hello");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svzTrailing = svzTest.trailing_end(2);

	EXPECT_EQ(svzTrailing.length(), 3);
	EXPECT_EQ(_tcscmp(svzTrailing, _T("llo")), 0);
}

TEST(zstring_view, trailing_end_from_middle)
{
	static constexpr auto pcszTestString = _T("testing");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svzTrailing = svzTest.trailing_end(4);

	EXPECT_EQ(svzTrailing.length(), 3);
	EXPECT_EQ(_tcscmp(svzTrailing, _T("ing")), 0);
}

TEST(zstring_view, as_string_view)
{
	static constexpr auto pcszTestString = _T("convert");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto sv = svzTest.asStringView();

	EXPECT_EQ(sv.length(), 7);
	EXPECT_TRUE(sv.compare(_T("convert")) == 0);
}

TEST(zstring_view, to_std_string)
{
	static constexpr auto pcszTestString = _T("convert");
	auto svzTest = vlr::tzstring_view{ pcszTestString };
	auto sConverted = svzTest.toStdString();

	EXPECT_EQ(sConverted.length(), 7);
	EXPECT_EQ(sConverted, std::basic_string<TCHAR>{ _T("convert") });
}

TEST(zstring_view, as_const_ptr)
{
	static constexpr auto pcszTestString = _T("pointer");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	auto pcszPtr = svzTest.asConstPtr();

	EXPECT_EQ(pcszPtr, pcszTestString);
}

TEST(zstring_view, implicit_const_pointer_cast)
{
	static constexpr auto pcszTestString = _T("implicit");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	const TCHAR* pcszPtr = svzTest;

	EXPECT_EQ(pcszPtr, pcszTestString);
}

TEST(zstring_view, from_std_string)
{
	std::basic_string<TCHAR> sTest{ _T("from_string") };
	auto svzTest = vlr::tzstring_view{ sTest };

	EXPECT_EQ(svzTest.length(), sTest.length());
	EXPECT_EQ(_tcscmp(svzTest, sTest.c_str()), 0);
}

TEST(zstring_view, char_specialization_from_std_string)
{
	std::string sTest{ "from_string" };
	auto svzTest = vlr::zstring_view{ sTest };

	EXPECT_EQ(svzTest.length(), sTest.length());
	EXPECT_EQ(strcmp(svzTest, sTest.c_str()), 0);
}

TEST(zstring_view, wchar_specialization_from_std_string)
{
	std::wstring sTest{ L"from_wstring" };
	auto svzTest = vlr::wzstring_view{ sTest };

	EXPECT_EQ(svzTest.length(), sTest.length());
	EXPECT_EQ(wcscmp(svzTest, sTest.c_str()), 0);
}

// Tests for basic_zstring_view_param
// The key difference: zstring_view_param allows construction from rvalue references to std::basic_string

TEST(zstring_view_param, rvalue_string_construction)
{
	// This test demonstrates the key differentiator: accepting temporary strings
	auto svzTest = vlr::tzstring_view_param{ std::basic_string<TCHAR>{ _T("temporary") } };

	EXPECT_EQ(svzTest.length(), 9);
	// Note: Any tests beyond this would be _unsafe_, as the string reference is dangling. This is not the 
	// intended usage, and this is by-design for this type (when constructed from a temporary string).
}

TEST(zstring_view_param, rvalue_char_string)
{
	auto svzTest = vlr::zstring_view_param{ std::string{ "rvalue_char" } };

	EXPECT_EQ(svzTest.length(), 11);
	// Note: Any tests beyond this would be _unsafe_, as the string reference is dangling. This is not the 
	// intended usage, and this is by-design for this type (when constructed from a temporary string).
}

TEST(zstring_view_param, rvalue_wchar_string)
{
	auto svzTest = vlr::wzstring_view_param{ std::wstring{ L"rvalue_wchar" } };

	EXPECT_EQ(svzTest.length(), 12);
	// Note: Any tests beyond this would be _unsafe_, as the string reference is dangling. This is not the 
	// intended usage, and this is by-design for this type.
}

TEST(zstring_view_param, lvalue_string_construction)
{
	// zstring_view_param should also work with lvalue strings
	std::basic_string<TCHAR> sTest{ _T("lvalue") };
	auto svzTest = vlr::tzstring_view_param{ sTest };

	EXPECT_EQ(svzTest.length(), 6);
	EXPECT_EQ(_tcscmp(svzTest, _T("lvalue")), 0);
}

TEST(zstring_view_param, null_pointer_construction)
{
	// Like base class, should accept null pointer
	auto svzTest = vlr::tzstring_view_param{ nullptr };

	EXPECT_EQ(svzTest.length(), 0);
	EXPECT_EQ(svzTest.data(), nullptr);
}

TEST(zstring_view_param, empty_string_construction)
{
	static constexpr auto pcszEmpty = _T("");
	auto svzTest = vlr::tzstring_view_param{ pcszEmpty };

	EXPECT_EQ(svzTest.length(), 0);
	EXPECT_EQ(svzTest.data()[0], _T('\0'));
}

TEST(zstring_view_param, trailing_end)
{
	static constexpr auto pcszTestString = _T("trailing_param");
	auto svzTest = vlr::tzstring_view_param{ pcszTestString };
	auto svzTrailing = svzTest.trailing_end(4);

	EXPECT_EQ(svzTrailing.length(), 10);
	EXPECT_EQ(_tcscmp(svzTrailing, _T("ling_param")), 0);
}

TEST(zstring_view_param, trailing_end_returns_param_type)
{
	// Verify that trailing_end returns the _param variant
	static constexpr auto pcszTestString = _T("test");
	auto svzTest = vlr::tzstring_view_param{ pcszTestString };
	auto svzTrailing = svzTest.trailing_end(1);

	EXPECT_EQ(svzTrailing.length(), 3);
}

TEST(zstring_view_param, as_string_view)
{
	static constexpr auto pcszTestString = _T("view_convert");
	auto svzTest = vlr::tzstring_view_param{ pcszTestString };
	auto sv = svzTest.asStringView();

	EXPECT_EQ(sv.length(), 12);
	EXPECT_TRUE(sv.compare(_T("view_convert")) == 0);
}

TEST(zstring_view_param, to_std_string)
{
	std::basic_string<TCHAR> sSource{ _T("to_convert") };
	auto svzTest = vlr::tzstring_view_param{ sSource };
	auto sConverted = svzTest.toStdString();

	EXPECT_EQ(sConverted, sSource);
}

TEST(zstring_view_param, copy_construction)
{
	auto svzOriginal = vlr::tzstring_view_param{ _T("original") };
	auto svzCopy = svzOriginal;

	EXPECT_EQ(svzCopy.length(), svzOriginal.length());
	EXPECT_EQ(svzCopy.data(), svzOriginal.data());
}

TEST(zstring_view_param, from_temporary_function_result)
{
	// Helper lambda that returns a temporary string
	auto getTempString = []() { return std::basic_string<TCHAR>{ _T("from_function") }; };

	// This should compile and work with zstring_view_param
	// (but would fail to compile with regular zstring_view)
	auto svzTest = vlr::tzstring_view_param{ getTempString() };

	EXPECT_EQ(svzTest.length(), 13);
	// Note: Any tests beyond this would be _unsafe_, as the string reference is dangling. This is not the 
	// intended usage, and this is by-design for this type.
}

// Additional corner case tests for zstring_view

TEST(zstring_view, single_character_string)
{
	static constexpr auto pcszTestString = _T("x");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	EXPECT_EQ(svzTest.length(), 1);
	EXPECT_EQ(_tcscmp(svzTest, _T("x")), 0);
}

TEST(zstring_view, very_long_string)
{
	// Create a long string to test performance and correctness with larger strings
	std::basic_string<TCHAR> sLongString(1000, _T('a'));
	auto svzTest = vlr::tzstring_view{ sLongString };

	EXPECT_EQ(svzTest.length(), 1000);
	EXPECT_EQ(svzTest[0], _T('a'));
	EXPECT_EQ(svzTest[999], _T('a'));
}

TEST(zstring_view, string_with_special_characters)
{
	static constexpr auto pcszTestString = _T("!@#$%^&*(){}[]|\\:;\"'<>,.?/~`");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	EXPECT_GT(svzTest.length(), 0);
	EXPECT_EQ(_tcscmp(svzTest, pcszTestString), 0);
}

TEST(zstring_view, string_with_whitespace)
{
	static constexpr auto pcszTestString = _T("  tabs\t\there  \n\nnewlines  ");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	EXPECT_GT(svzTest.length(), 0);
	EXPECT_EQ(_tcscmp(svzTest, pcszTestString), 0);
}

TEST(zstring_view, trailing_end_zero_offset)
{
	static constexpr auto pcszTestString = _T("start");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svzTrailing = svzTest.trailing_end(0);

	EXPECT_EQ(svzTrailing.length(), 5);
	EXPECT_EQ(_tcscmp(svzTrailing, pcszTestString), 0);
}

TEST(zstring_view, trailing_end_at_end)
{
	static constexpr auto pcszTestString = _T("ending");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svzTrailing = svzTest.trailing_end(6);

	EXPECT_EQ(svzTrailing.length(), 0);
}

TEST(zstring_view, string_view_comparison)
{
	static constexpr auto pcszTestString = _T("comparable");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svTest = vlr::tstring_view{ pcszTestString };

	// Both should have same data and length
	EXPECT_EQ(svzTest.data(), svTest.data());
	EXPECT_EQ(svzTest.length(), svTest.length());
	// Content should be identical
	EXPECT_EQ(svzTest.compare(svTest), 0);
}

TEST(zstring_view, iterator_operations)
{
	static constexpr auto pcszTestString = _T("iterate");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	// Test that we can iterate
	auto it = svzTest.begin();
	EXPECT_EQ(*it, _T('i'));
	EXPECT_EQ(*(svzTest.end() - 1), _T('e'));
}

TEST(zstring_view, null_pointer_access)
{
	auto svzTest = vlr::tzstring_view{ nullptr };

	EXPECT_EQ(svzTest.length(), 0);
	EXPECT_EQ(svzTest.data(), nullptr);
	// Implicit conversion should also be null
	const TCHAR* pcszPtr = svzTest;
	EXPECT_EQ(pcszPtr, nullptr);
}

TEST(zstring_view, string_with_embedded_nulls)
{
	// While zstring_view is designed for null-terminated strings, we should handle
	// the case where we're constructed with an explicit length that includes embedded nulls
	static constexpr auto pcszTestString = _T("hel\0lo");
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString, 6, vlr::tzstring_view::StringIsNullTerminated{} };

	// The view should only see up to the first embedded null when using pointer-only constructor
	auto svzPointerOnly = vlr::tzstring_view{ pcszTestString };
	EXPECT_EQ(svzPointerOnly.length(), 3);  // Only "hel"
}
