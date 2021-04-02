#include "pch.h"

#include "vlr/util.logical_zstring_view.h"

TEST( util_logical_zstring_view, general )
{
	using namespace vlr::util;

	{
		auto svzValue = logical_azstring_view{};
		EXPECT_STREQ( svzValue, NULL );
	}
	{
		auto svzValue = logical_azstring_view{ "test" };
		EXPECT_STREQ( svzValue, "test" );
	}
	{
		auto svzValue = logical_wzstring_view{};
		EXPECT_STREQ( svzValue, NULL );
	}
	{
		auto svzValue = logical_wzstring_view{ L"test" };
		EXPECT_STREQ( svzValue, L"test" );
	}
	{
		auto svzValue = logical_tzstring_view{};
		EXPECT_STREQ( svzValue, NULL );
	}
	{
		auto svzValue = logical_tzstring_view{ _T( "test" ) };
		EXPECT_STREQ( svzValue, _T( "test" ) );
	}
	{
		auto sValue = vlr::tstring{ _T( "test" ) };
		auto svzValue = logical_tzstring_view{ sValue };
		EXPECT_STREQ( svzValue, _T( "test" ) );
	}
	{
		auto sValue = vlr::tstring{ _T( "test" ) };
		auto svzValue = logical_tzstring_view{};
		svzValue = sValue;
		EXPECT_STREQ( svzValue, _T( "test" ) );
	}
}
