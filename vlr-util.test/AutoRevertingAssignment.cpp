#include "pch.h"

#include "../vlr-util/AutoRevertingAssignment.h"

TEST( CAutoRevertingAssignment, general )
{
	int nValue = 0;
	{
		auto oAutoRevert = vlr::MakeAutoRevertingAssignment( nValue, 42 );
		EXPECT_EQ( nValue, 42 );
	}
	EXPECT_EQ( nValue, 0 );

	std::string strValue;
	{
		auto oAutoRevert = vlr::MakeAutoRevertingAssignment( strValue, "hello" );
		EXPECT_STREQ( strValue.c_str(), "hello" );
	}
	EXPECT_STREQ( strValue.c_str(), "" );
}
