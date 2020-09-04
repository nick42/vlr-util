#include "pch.h"

#include "../vlr-util/enums_RangeInfo.h"

TEST( enums_RangeInfo, sequential_range_iterator )
{
	using TIterator_int_0to10 = vlr::enums::sequential_range_iterator<int, 10, 0>;
	using TRangeInfo_unsigned_0to10 = vlr::enums::CRangeInfoSequential_Unsigned_Default<10>;
	using TRangeInfo_signed_0to10 = vlr::enums::CRangeInfoSequential_Signed_Default<10>;

	{
		auto iter0to10 = TIterator_int_0to10{};
		//EXPECT_ANY_THROW( *iter0to10 );
		//EXPECT_ANY_THROW( ++iter0to10 );
		//EXPECT_ANY_THROW( --iter0to10 );
		//EXPECT_ANY_THROW( iter0to10++ );
		//EXPECT_ANY_THROW( iter0to10-- );
	}
	{
		auto iter0to10 = TIterator_int_0to10{ 0 };
		EXPECT_EQ( *iter0to10, 0 );
		auto iterValue0 = iter0to10++;
		EXPECT_EQ( *iterValue0, 0 );
		EXPECT_EQ( *iter0to10, 1 );
		auto iterValue1 = iter0to10--;
		EXPECT_EQ( *iterValue1, 1 );
		EXPECT_EQ( *iter0to10, 0 );
		//EXPECT_ANY_THROW( --iter0to10 );
	}

	{
		auto oRangeInfo0to10 = TRangeInfo_unsigned_0to10{};
		unsigned int nCheckValue = 0;
		for (auto nValue : oRangeInfo0to10)
		{
			EXPECT_EQ( nValue, nCheckValue++ );
		}
		EXPECT_EQ( nCheckValue, 11 );
	}
	{
		auto oRangeInfo0to10 = TRangeInfo_signed_0to10{};
		int nCheckValue = 0;
		for (auto nValue : oRangeInfo0to10)
		{
			EXPECT_EQ( nValue, nCheckValue++ );
		}
		EXPECT_EQ( nCheckValue, 11 );
	}
}
