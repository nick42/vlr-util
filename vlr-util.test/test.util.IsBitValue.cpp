#include "pch.h"

#include "vlr-util/util.IsBitValue.h"

TEST( util_IsBitValue, general )
{
	{
		constexpr auto bIsSingleBitValue = vlr::util::IsSingleBitValue( 0 );
		EXPECT_EQ( bIsSingleBitValue, false );
	}
	EXPECT_EQ( vlr::util::IsSingleBitValue( 1 ), true );
	EXPECT_EQ( vlr::util::IsSingleBitValue( 1 << 1 ), true );
	EXPECT_EQ( vlr::util::IsSingleBitValue( 3 ), false );
	EXPECT_EQ( vlr::util::IsSingleBitValue( 42 ), false );
}
