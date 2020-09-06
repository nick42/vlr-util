#include "pch.h"

#include "vlr/formatpf.h"

TEST( formatpf, general )
{
	{
		auto sResult = vlr::formatpf_to<CStringA>( "The answer is %d", 42 );
		EXPECT_STREQ( sResult, "The answer is 42" );
	}
	{
		auto sResult = vlr::formatpf_to<CStringW>( L"The answer is %d", 42 );
		EXPECT_STREQ( sResult, L"The answer is 42" );
	}
	{
		auto sResult = vlr::formatpf_to<CString>( _T( "The answer is %d" ), 42 );
		EXPECT_STREQ( sResult, _T( "The answer is 42" ) );
	}
	{
		auto sResult = vlr::formatpf_to<std::string>( "The answer is %d", 42 );
		EXPECT_STREQ( sResult.c_str(), "The answer is 42" );
	}
	{
		auto sResult = vlr::formatpf_to<std::wstring>( L"The answer is %d", 42 );
		EXPECT_STREQ( sResult.c_str(), L"The answer is 42" );
	}
	{
		auto sResult = vlr::formatpf_to<vlr::tstring>( _T( "The answer is %d" ), 42 );
		EXPECT_STREQ( sResult.c_str(), _T( "The answer is 42" ) );
	}
}
