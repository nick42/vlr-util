#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"
#include "util.IsBitSet.h"
#include "util.range_checked_cast.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(util)

VLR_NAMESPACE_BEGIN(detail)

static constexpr auto Severity_Success = (unsigned char)0;
static constexpr auto Severity_Failure = (unsigned char)1;

static constexpr auto Facility_CallSpecific = (unsigned short)4;
static constexpr auto Facility_Win32 = (unsigned short)7;

inline constexpr auto MakeResultCode(unsigned char nSeverityCode, unsigned short nFacilityCode, unsigned short nResultCode)
{
	return ResultCode{ ((unsigned long)(nSeverityCode) << 31) | ((unsigned long)(nFacilityCode) << 16) | ((unsigned long)(nResultCode)) };
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
inline constexpr auto MakeResultCode_Failure_CallSpecific(unsigned long nResultCodeLong)
{
	auto nResultCode = range_checked_cast<unsigned short>(nResultCodeLong);
	return MakeResultCode(Severity_Failure, Facility_CallSpecific, nResultCode);
}
inline constexpr auto MakeResultCode_Failure_Win32(unsigned short nResultCode)
{
	return MakeResultCode(Severity_Failure, Facility_Win32, nResultCode);
}
inline constexpr auto MakeResultCode_Failure_Win32(unsigned long nResultCodeLong)
{
	auto nResultCode = range_checked_cast<unsigned short>(nResultCodeLong);
	return MakeResultCode(Severity_Failure, Facility_Win32, nResultCode);
}

VLR_NAMESPACE_END //(detail)

class SResult
{
public:
	static constexpr auto Uninitialized = HRESULT{0x0000FFFF};
	static constexpr auto Success = S_OK;
	static constexpr auto Success_WithNuance = S_FALSE;
	static constexpr auto Success_NoWorkDone = S_FALSE;
	static constexpr auto Failure = E_FAIL;

protected:
	ResultCode m_nResultCode = Uninitialized;

public:
	decltype(auto) withHRESULT(HRESULT hrResult)
	{
		m_nResultCode = hrResult;
		return *this;
	}

public:
	inline operator ResultCode() const
	{
		return m_nResultCode;
	}

public:
	static inline auto ForGeneralSuccess()
	{
		return SResult{ S_OK };
	}
	static inline auto ForSuccessWithNuance()
	{
		return SResult{ S_FALSE };
	}
	static inline auto ForGeneralFailure()
	{
		return SResult{ E_FAIL };
	}

	static inline auto For_win32_ErrorCode(DWORD dwErrorCode)
	{
		return SResult{}.withHRESULT(detail::MakeResultCode_Failure_Win32(dwErrorCode));
	}
	static inline auto For_win32_LastError()
	{
		auto dwLastError = ::GetLastError();
		return For_win32_ErrorCode(dwLastError);
	}
	static inline auto ForCall_win32(BOOL bSuccess)
	{
		if (bSuccess)
		{
			return ForGeneralSuccess();
		}
		else
		{
			return For_win32_LastError();
		}
	}

	static inline auto ForCallSpecificResult(unsigned long nResultCode)
	{
		return SResult{ detail::MakeResultCode_Failure_CallSpecific(nResultCode) };
	}

public:
	constexpr auto isSuccess() const
	{
		return !IsBitSet(m_nResultCode, 0x80000000);
	}
	constexpr auto isFailure() const
	{
		return IsBitSet(m_nResultCode, 0x80000000);
	}

public:
	vlr::tstring ToString() const;

public:
	constexpr SResult() = default;
	constexpr SResult(HRESULT hrResult)
		: m_nResultCode{ hrResult }
	{}
};

VLR_NAMESPACE_END //(util)

using util::SResult;
using StandardResult = SResult;

VLR_NAMESPACE_END //(vlr)
