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

// Note: This code generates an INTERNAL COMPILER ERROR in msvc 17.14.13
//TEST(util_Result, ExpectedMethodsAreNothrow)
//{
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::withHRESULT), SResult&, HRESULT>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::asHRESULT), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::asWin32Code), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::operator ResultCode), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::ForGeneralSuccess)>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::ForSuccessWithNuance)>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::ForGeneralFailure)>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::ForHRESULT), HRESULT>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::For_win32_GeneralResultCode), DWORD, vlr::util::Result::SourceTypeHint::ESourceTypeHint>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::For_win32_ErrorCode), DWORD>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::For_win32_LastError)>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::ForCall_win32), BOOL>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::For_win32_SEHExceptionCode), DWORD>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::For_win32_SEHExceptionCode_RPC), DWORD>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::ForCallSpecificResult), unsigned long>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::isSuccess), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::isFailure), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::isSet), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::GetFacilityCode), const SResult&>);
//	static_assert(std::is_nothrow_invocable_v<decltype(SResult::GetUnqualifiedResultCode), const SResult&>);
//
//	static_assert(std::is_nothrow_constructible_v<SResult>);
//	static_assert(std::is_nothrow_constructible_v<SResult, HRESULT>);
//}

#endif // defined(WIN32)
