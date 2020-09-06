#include "pch.h"

#include "vlr/util.range_checked_cast.h"

TEST( util_range_checked_cast, general )
{
	using namespace vlr::util;

	EXPECT_EQ( range_checked_cast<int>(1UL), 1 );
}
