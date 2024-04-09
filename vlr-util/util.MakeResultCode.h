#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"
#include "util.range_checked_cast.h"

namespace vlr {

namespace util {

namespace detail {

static constexpr auto Severity_Success = (unsigned char)0;
static constexpr auto Severity_Failure = (unsigned char)1;

static constexpr auto Facility_CallSpecific = (unsigned short)4;
static constexpr auto Facility_Win32 = (unsigned short)7;

inline constexpr auto MakeResultCode(unsigned char nSeverityCode, unsigned short nFacilityCode, unsigned short nResultCode)
{
	// Note: Ensure we're using 32bit uint explicitly; on other platforms, long may be 64bit
	return ResultCode{ ((uint32_t)(nSeverityCode) << 31) | ((uint32_t)(nFacilityCode) << 16) | ((uint32_t)(nResultCode)) };
}

inline constexpr auto MakeResultCode_Success(unsigned short nFacilityCode, unsigned short nResultCode)
{
	return MakeResultCode(Severity_Success, nFacilityCode, nResultCode);
}
inline constexpr auto MakeResultCode_Failure(unsigned short nFacilityCode, unsigned short nResultCode)
{
	return MakeResultCode(Severity_Failure, nFacilityCode, nResultCode);
}

inline constexpr auto MakeResultCode_Failure_CallSpecific(unsigned short nResultCode)
{
	return MakeResultCode(Severity_Failure, Facility_CallSpecific, nResultCode);
}
template <typename TCode, typename std::enable_if_t<std::is_integral_v<TCode>>* = nullptr>
inline constexpr auto MakeResultCode_Failure_CallSpecific(TCode nResultCodeLong)
{
	auto nResultCode = vlr::util::range_checked_cast<unsigned short>(nResultCodeLong);
	return MakeResultCode(Severity_Failure, Facility_CallSpecific, nResultCode);
}
inline constexpr auto MakeResultCode_Failure_Win32(unsigned short nResultCode)
{
	return MakeResultCode(Severity_Failure, Facility_Win32, nResultCode);
}
template <typename TCode, typename std::enable_if_t<std::is_integral_v<TCode>>* = nullptr>
inline constexpr auto MakeResultCode_Failure_Win32(TCode nResultCodeLong)
{
	auto nResultCode = vlr::util::range_checked_cast<unsigned short>(nResultCodeLong);
	return MakeResultCode(Severity_Failure, Facility_Win32, nResultCode);
}

} // namespace detail

} // namespace util

} // namespace vlr
