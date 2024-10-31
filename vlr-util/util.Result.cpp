#include "pch.h"
#include "util.Result.h"

#if defined(WIN32)
#include <NTSecAPI.h>
#endif

#include "formatpf.h"

namespace vlr {

namespace util {

#if defined(WIN32)

SResult SResult::For_win32_GeneralResultCode(DWORD dwResultCode,
	Result::SourceTypeHint::ESourceTypeHint eSourceTypeHint /*= Result::SourceTypeHint::Unknown*/)
{
	// For general result codes, the caller may pass a "success" value. For the general case,
	// we will only consider 0 for success.
	if (dwResultCode == 0)
	{
		return Success;
	}

	// If the result code is in range for Win32 error code values, and doesn't have any high bits set, 
	// we will assume that this is a Win32 error code.
	if (dwResultCode <= std::numeric_limits<unsigned short>::max())
	{
		return SResult{ detail::MakeResultCode_Failure_Win32(dwResultCode) };
	}

	// Note: Beyond this point, the code had some high bits set, which means it is either already a HRESULT,
	// or another format with high bits. Evaluations are based on the hint type.

	const auto srNominalResultCode = SResult{ static_cast<HRESULT>(dwResultCode) };
	const auto nNominalFacilityCode = srNominalResultCode.GetFacilityCode();
	const auto nNominalUnqualifiedResultCode = srNominalResultCode.GetUnqualifiedResultCode();
	const bool bIsFailureBitSet = srNominalResultCode.isFailure();

	if (eSourceTypeHint == Result::SourceTypeHint::Win32_SEHException_RPC)
	{
		// If the code has the RPC facility bit set, then we will assume this is already a formatted RPC HRESULT,
		// and pass it through, just ensuring that the error bit is set.
		if ((nNominalFacilityCode != 0)
			&& (nNominalFacilityCode == detail::Facility_RPC))
		{
			return detail::MakeResultCode(detail::Severity_Failure, nNominalFacilityCode, nNominalUnqualifiedResultCode);
		}

		// If not, we will treat this an a code from SEH, and handle it as that below
		eSourceTypeHint = Result::SourceTypeHint::Win32_SEHException;
	}

	bool bFailureCodeWithoutFacilityCode = bIsFailureBitSet && (nNominalFacilityCode == 0);
	// Note: If we have a high bit but no facility code, or we have a SEH exception hint, try as NTSTATUS value.
	bool bEvaluateAsPotentialNTSTATUS = bFailureCodeWithoutFacilityCode
		|| (eSourceTypeHint == Result::SourceTypeHint::Win32_SEHException);

	if (bEvaluateAsPotentialNTSTATUS)
	{
		// Win32 SEH exceptions are usually NTSTATUS values. There's an API to convert these to HRESULTS.
		auto nWin32ResultCode = LsaNtStatusToWinError(dwResultCode);
		if ((nWin32ResultCode != ERROR_MR_MID_NOT_FOUND)
			// Note: Must also check that the result value is in the range for Win32 result codes, else we 
			// might create a recursive call here.
			&& (nWin32ResultCode <= std::numeric_limits<unsigned short>::max()))
		{
			// We found a conversion to a Win32 error code; wrap as such
			return For_win32_ErrorCode(nWin32ResultCode);
		}
	}

	// If we didn't get any conversions based on the hints above, then just pass through as an error code, only 
	// setting the error bit as such.
	return detail::MakeResultCode(detail::Severity_Failure, nNominalFacilityCode, nNominalUnqualifiedResultCode);
}

SResult SResult::For_win32_SEHExceptionCode(DWORD dwExceptionCode)
{
	return For_win32_GeneralResultCode(dwExceptionCode, Result::SourceTypeHint::Win32_SEHException);
}

SResult SResult::For_win32_SEHExceptionCode_RPC(DWORD dwExceptionCode)
{
	return For_win32_GeneralResultCode(dwExceptionCode, Result::SourceTypeHint::Win32_SEHException_RPC);
}

#endif // defined(WIN32)

vlr::tstring SResult::ToString() const
{
	return vlr::formatpf(_T("0x%08X"), m_nResultCode);
}

} // namespace util

} // namespace vlr
