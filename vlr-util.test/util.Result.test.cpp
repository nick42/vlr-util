#include "pch.h"

#include <vlr-util/util.Result.h>

#if defined(WIN32)

using vlr::util::SResult;

TEST(util_Result, For_win32_GeneralResultCode_0)
{
	DWORD nTestValue = 0;

	auto srResult = SResult::For_win32_GeneralResultCode(nTestValue);
	EXPECT_EQ(srResult, SResult::Success);
}

TEST(util_Result, For_win32_GeneralResultCode_1)
{
	DWORD nTestValue = 1;

	auto srResult = SResult::For_win32_GeneralResultCode(nTestValue);
	EXPECT_TRUE(srResult.isFailure());
	EXPECT_EQ(srResult.GetFacilityCode(), vlr::util::detail::Facility_Win32);
	EXPECT_EQ(srResult.GetUnqualifiedResultCode(), 1);
}

TEST(util_Result, For_win32_GeneralResultCode_NTSTATUS)
{
	DWORD nTestValue = EXCEPTION_ACCESS_VIOLATION;

	auto srResult = SResult::For_win32_GeneralResultCode(nTestValue);
	EXPECT_TRUE(srResult.isFailure());
	EXPECT_EQ(srResult.GetFacilityCode(), vlr::util::detail::Facility_Win32);
	EXPECT_EQ(srResult.GetUnqualifiedResultCode(), ERROR_NOACCESS);
}

TEST(util_Result, For_win32_GeneralResultCode_PassthroughForRPC)
{
	DWORD nTestValue = RPC_E_ACCESS_DENIED;

	auto srResult = SResult::For_win32_GeneralResultCode(nTestValue);
	EXPECT_TRUE(srResult.isFailure());
	EXPECT_EQ(srResult, RPC_E_ACCESS_DENIED);
}

#endif // defined(WIN32)
