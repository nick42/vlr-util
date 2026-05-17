#include "pch.h"

#include "vlr-util/AutoRevertingAssignment.h"

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

// Test if we can set a type with a different typed value (as long as assignment operator works)

TEST(CAutoRevertingAssignment, std_atomic_bool)
{
	auto atomValue = std::atomic_bool{ false };
	{
		auto oAutoRevert = vlr::MakeAutoRevertingAssignment(atomValue, true);
		EXPECT_EQ(atomValue.load(), true);
	}
	EXPECT_EQ(atomValue.load(), false);
}
