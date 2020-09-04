#include "pch.h"

#include "../vlr-util/formatpf.h"

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
}
