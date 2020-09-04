#include "pch.h"

#include "../vlr-util/util.IsNonZero.h"

TEST( util_IsNonZero, general )
{
	EXPECT_EQ( vlr::util::IsNonZero( 0 ), false );
	EXPECT_EQ( vlr::util::IsNonZero( 1 ), true );
	EXPECT_EQ( vlr::util::IsNonZero( -1 ), true );
	EXPECT_EQ( vlr::util::IsNonZero( 0U ), false );
	EXPECT_EQ( vlr::util::IsNonZero( 1U ), true );
	EXPECT_EQ( vlr::util::IsNonZero( (void*)0 ), false );
	EXPECT_EQ( vlr::util::IsNonZero( (void*)0xBAADF00D ), true );
	EXPECT_EQ( vlr::util::IsNonZero( [] { return; } ), true );
	EXPECT_EQ( vlr::util::IsNonZero( std::function<void()>{} ), false );
	EXPECT_EQ( vlr::util::IsNonZero( std::function<void()>{ [] { return; } } ), true );
}
