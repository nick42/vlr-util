#include "pch.h"
#include "zstring_view.h"

#include <gtest/gtest.h>

#include "../vlr-util/zstring_view.h"

static constexpr auto pcszTestStringA = "test";
static constexpr auto pcszTestStringW = L"test";
static constexpr auto pcszTestStringT = _T( "test" );

TEST( zstring_view, char_base )
{
	static constexpr auto pcszTestString = pcszTestStringA;
	auto nTestStringLen = strlen( pcszTestString );
	auto svzTest = vlr::zstring_view{ pcszTestString };

	EXPECT_EQ( svzTest.length(), nTestStringLen );
	EXPECT_EQ( strcmp( svzTest, pcszTestString ), 0 );
	// Assure svz is actually NULL-terminated
	auto pcszTest = static_cast<const char*>(svzTest);
	EXPECT_EQ( pcszTest[nTestStringLen], '\0' );
}

TEST( zstring_view, wchar_base )
{
	static constexpr auto pcszTestString = pcszTestStringW;
	auto nTestStringLen = wcslen( pcszTestString );
	auto svzTest = vlr::wzstring_view{ pcszTestString };

	EXPECT_EQ( svzTest.length(), wcslen( pcszTestString ) );
	EXPECT_EQ( wcscmp( svzTest, pcszTestString ), 0 );
	// Assure svz is actually NULL-terminated
	auto pcszTest = static_cast<const wchar_t*>(svzTest);
	EXPECT_EQ( pcszTest[nTestStringLen], L'\0' );
}

TEST( zstring_view, constexpr_base )
{
	static constexpr auto pcszTestString = pcszTestStringT;
	auto nTestStringLen = _tcslen( pcszTestString );
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };

	EXPECT_EQ( svzTest.length(), _tcslen( pcszTestString ) );
	EXPECT_EQ( _tcscmp( svzTest, pcszTestString ), 0 );
	// Assure svz is actually NULL-terminated
	auto pcszTest = static_cast<const TCHAR*>(svzTest);
	EXPECT_EQ( pcszTest[nTestStringLen], _T( '\0' ) );
}

// zstring_view should be functionally identical to std::string_view, in terms of its semantics and methods

TEST( zstring_view, inherited_behavior )
{
	static constexpr auto pcszTestString = pcszTestStringT;
	constexpr auto svzTest = vlr::tzstring_view{ pcszTestString };
	constexpr auto svText = vlr::tstring_view{ pcszTestString };

	EXPECT_EQ( svzTest.data(), svText.data() );
	EXPECT_EQ( svzTest.length(), svText.length() );
}
