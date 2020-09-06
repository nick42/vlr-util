#include "pch.h"

#include "vlr/util.convert.StringConversion.h"

TEST( util_convert_StringConversion, general )
{
	using namespace vlr::util;

	const auto strFoo = std::string{ "foo" };
	const auto strwFoo = std::wstring{ L"foo" };

	{
		auto strFoo_A = Convert::ToStdStringA( strFoo );
		EXPECT_STREQ( strFoo_A.c_str(), "foo" );
		auto strFoo_W = Convert::ToStdStringW( strFoo );
		EXPECT_STREQ( strFoo_W.c_str(), L"foo" );
		auto strFoo_T = Convert::ToStdString( strFoo );
		EXPECT_STREQ( strFoo_T.c_str(), _T( "foo" ) );
	}
	{
		auto strFoo_A = Convert::ToStdStringA( strwFoo );
		EXPECT_STREQ( strFoo_A.c_str(), "foo" );
		auto strFoo_W = Convert::ToStdStringW( strwFoo );
		EXPECT_STREQ( strFoo_W.c_str(), L"foo" );
		auto strFoo_T = Convert::ToStdString( strwFoo );
		EXPECT_STREQ( strFoo_T.c_str(), _T( "foo" ) );
	}
}
