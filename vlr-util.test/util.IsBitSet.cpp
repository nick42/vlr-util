#include "pch.h"

#include "vlr-util/util.IsBitSet.h"

TEST( util_IsBitSet, general )
{
	static constexpr auto nTestValue_UL_0 = 0UL;
	static constexpr auto nTestValue_UL_1 = 1UL;
	static constexpr auto nTestValue_UL_0x01010101 = 0x01010101UL;

	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_0, 1UL ), false );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_0, 1 ), false );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_1, 1UL ), true );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_1, 1 ), true );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_0x01010101, 1UL ), true );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_0x01010101, 1 ), true );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_0x01010101, 0x10UL ), false );
	EXPECT_EQ( vlr::util::IsBitSet( nTestValue_UL_0x01010101, 0x10 ), false );
}
