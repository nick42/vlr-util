#include "pch.h"

#include "vlr/util.IsBitSet.h"

TEST( util_IsBitSet, general )
{
	EXPECT_EQ( vlr::util::IsBitSet( 0UL, 1UL ), false );
	EXPECT_EQ( vlr::util::IsBitSet( 1UL, 1UL ), true );
}
