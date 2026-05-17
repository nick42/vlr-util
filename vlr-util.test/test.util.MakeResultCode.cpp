#include "pch.h"

#include <vlr-util/util.MakeResultCode.h>

using vlr::util::detail::MakeResultCode_Failure_CallSpecific;

TEST(util_MakeResultCode, general)
{
	static constexpr auto srReference = MakeResultCode_Failure_CallSpecific((unsigned short)1);

	static constexpr auto srFromChar = MakeResultCode_Failure_CallSpecific((char)1);
	static constexpr auto srFromUChar = MakeResultCode_Failure_CallSpecific((unsigned char)1);
	static constexpr auto srFromShort = MakeResultCode_Failure_CallSpecific((short)1);
	static constexpr auto srFromUShort = MakeResultCode_Failure_CallSpecific((unsigned short)1);
	static constexpr auto srFromInt = MakeResultCode_Failure_CallSpecific((int)1);
	static constexpr auto srFromUInt = MakeResultCode_Failure_CallSpecific((unsigned int)1);
	static constexpr auto srFromLong = MakeResultCode_Failure_CallSpecific((long)1);
	static constexpr auto srFromULong = MakeResultCode_Failure_CallSpecific((unsigned long)1);
	static constexpr auto srFromLongLong = MakeResultCode_Failure_CallSpecific((long long)1);
	static constexpr auto srFromULongLong = MakeResultCode_Failure_CallSpecific((unsigned long long)1);

	EXPECT_EQ(srReference, srFromChar);
	EXPECT_EQ(srReference, srFromUChar);
	EXPECT_EQ(srReference, srFromShort);
	EXPECT_EQ(srReference, srFromUShort);
	EXPECT_EQ(srReference, srFromInt);
	EXPECT_EQ(srReference, srFromUInt);
	EXPECT_EQ(srReference, srFromLong);
	EXPECT_EQ(srReference, srFromULong);
	EXPECT_EQ(srReference, srFromLongLong);
	EXPECT_EQ(srReference, srFromULongLong);
}
