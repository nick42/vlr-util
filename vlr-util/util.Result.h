#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"
#include "util.IsBitSet.h"
#include "util.MakeResultCode.h"

namespace vlr {

namespace util {

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
	decltype(auto) asHRESULT() const
	{
		return m_nResultCode;
	}
	decltype(auto) asWin32Code() const
	{
		return (m_nResultCode & 0xFFFF);
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
	static inline auto ForHRESULT(HRESULT hr)
	{
		return SResult{ hr };
	}

#ifdef WIN32
	static inline auto For_win32_ErrorCode(DWORD dwErrorCode)
	{
		return SResult{}.withHRESULT(detail::MakeResultCode_Failure_Win32(dwErrorCode));
	}
	static inline auto For_win32_LastError()
	{
		auto dwLastError = ::GetLastError();
		return For_win32_ErrorCode(dwLastError);
	}
	static inline auto ForCall_win32(WIN_BOOL bSuccess)
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
#endif

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
	constexpr auto isSet() const
	{
		return m_nResultCode != Uninitialized;
	}

public:
	vlr::tstring ToString() const;

public:
	constexpr SResult() = default;
	constexpr SResult(HRESULT hrResult)
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
