#pragma once

#include "config.h"

#include "util.std_aliases.h"
#include "util.IsBitSet.h"
#include "util.MakeResultCode.h"

namespace vlr {

namespace util {

namespace Result {

// For use when doing error code conversions

namespace SourceTypeHint {

enum ESourceTypeHint
{
	Unknown,
	Win32_SEHException,
	Win32_SEHException_RPC,
};

} // namespace SourceType

} // namespace Result

class SResult
{
public:
	static constexpr auto Uninitialized = HRESULT{ 0x0000FFFF };
	static constexpr auto Success = S_OK;
	static constexpr auto Success_WithNuance = S_FALSE;
	static constexpr auto Success_NoWorkDone = S_FALSE;
	static constexpr auto Failure = E_FAIL;
	static constexpr auto Failure_NotImplemented = (HRESULT)0x80004001L; // E_NOTIMPL

protected:
	ResultCode m_nResultCode = Uninitialized;

public:
	inline decltype(auto) withHRESULT(HRESULT hrResult) noexcept
	{
		m_nResultCode = hrResult;
		return *this;
	}
	inline decltype(auto) asHRESULT() const noexcept
	{
		return (m_nResultCode);
	}
	inline decltype(auto) asWin32Code() const noexcept
	{
		return (m_nResultCode & 0xFFFF);
	}

public:
	inline operator ResultCode() const noexcept
	{
		return m_nResultCode;
	}

public:
	static inline auto ForGeneralSuccess() noexcept
	{
		return SResult{ S_OK };
	}
	static inline auto ForSuccessWithNuance() noexcept
	{
		return SResult{ S_FALSE };
	}
	static inline auto ForGeneralFailure() noexcept
	{
		return SResult{ E_FAIL };
	}
	static inline auto ForHRESULT(HRESULT hr) noexcept
	{
		return SResult{ hr };
	}

#if defined(WIN32)
	// This method tries to handle the general-case of conversion from Win32 DWORD result codes, which may be:
	// - 16bit Win32 error codes
	// - NTSTATUS value
	// - HRESULT value
	// - ... other
	static SResult For_win32_GeneralResultCode(DWORD dwResultCode,
		Result::SourceTypeHint::ESourceTypeHint eSourceTypeHint = Result::SourceTypeHint::Unknown) noexcept;
	static inline SResult For_win32_ErrorCode(DWORD dwErrorCode) noexcept
	{
		// Note: We assume that if this function is called, the result is an error, and treat even 0 as such.

		// Note: Win32 result codes are in a 16bit range. We should check this; if the value passed in 
		// is not in range, else we may zero out the value in the range checking code internally.
		// Note: This means we may change the logical result value if we pass something which is not a Win32 code.
		if (dwErrorCode <= std::numeric_limits<unsigned short>::max())
		{
			return SResult{ detail::MakeResultCode_Failure_Win32(dwErrorCode) };
		}

		// For this case, we could truncate, or try to interpret as appropriate.
		return For_win32_GeneralResultCode(dwErrorCode);
	}
	static inline SResult For_win32_LastError() noexcept
	{
		auto dwLastError = ::GetLastError();
		return For_win32_ErrorCode(dwLastError);
	}
	static inline SResult ForCall_win32(WIN_BOOL bSuccess) noexcept
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
	// Note: SEH exception codes can be Win32 codes, but are usually NTSTATUS values. These are similar, 
	// but not the same, as HRESULT values.
	// See: https://jpassing.com/2007/08/20/error-codes-win32-vs-hresult-vs-ntstatus/
	static SResult For_win32_SEHExceptionCode(DWORD dwExceptionCode) noexcept;
	// If we know the SEH exception is from RPC explicitly, then we can set the RPC facility bit.
	static SResult For_win32_SEHExceptionCode_RPC(DWORD dwExceptionCode) noexcept;
#endif // defined(WIN32)

	static inline auto ForCallSpecificResult(unsigned long nResultCode) noexcept
	{
		return SResult{ detail::MakeResultCode_Failure_CallSpecific(nResultCode) };
	}

public:
	constexpr auto isSuccess() const noexcept
	{
		return !IsBitSet(m_nResultCode, 0x80000000);
	}
	constexpr auto isFailure() const noexcept
	{
		return IsBitSet(m_nResultCode, 0x80000000);
	}
	constexpr auto isSet() const noexcept
	{
		return m_nResultCode != Uninitialized;
	}
	constexpr unsigned short GetFacilityCode() const noexcept
	{
		return (m_nResultCode & 0x07FF0000) >> 16;
	}
	constexpr unsigned short GetUnqualifiedResultCode() const noexcept
	{
		return static_cast<unsigned short>(m_nResultCode & 0xFFFF);
	}

public:
	vlr::tstring ToString() const;

public:
	constexpr SResult() noexcept = default;
	constexpr SResult(HRESULT hrResult) noexcept
		: m_nResultCode{ hrResult }
	{}
private:
	// Note: Block implicit conversion frm bool, as this is a potential pitfall
	constexpr SResult(bool /*bValue*/) {}
};

} // namespace util

using util::SResult;
using StandardResult = SResult;

} // namespace vlr
