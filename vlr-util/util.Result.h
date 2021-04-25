#pragma once

#include "UtilMacros.Namespace.h"

#include "util.std_aliases.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

class CResult
{
protected:
	HRESULT m_hrResult = S_OK;

public:
	decltype(auto) With_HRESULT( HRESULT hrResult )
	{
		m_hrResult = hrResult;
		return *this;
	}

public:
	inline operator HRESULT() const
	{
		return m_hrResult;
	}

public:
	static auto For_win32_LastError()
	{
		auto dwLastError = ::GetLastError();
		return CResult{}.With_HRESULT( HRESULT_FROM_WIN32( dwLastError ) );
	}
	static auto For_win32_ErrorCode( DWORD dwErrorCode )
	{
		return CResult{}.With_HRESULT( HRESULT_FROM_WIN32( dwErrorCode ) );
	}
	static auto ForSuccess()
	{
		return CResult{ S_OK };
	}
	static auto ForCall_win32( BOOL bSuccess )
	{
		if (bSuccess)
		{
			return ForSuccess();
		}
		else
		{
			return For_win32_LastError();
		}
	}

public:
	vlr::tstring ToString() const;

protected:
	constexpr CResult() = default;
public:
	constexpr CResult( HRESULT hrResult )
		: m_hrResult{ hrResult }
	{}
};

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
