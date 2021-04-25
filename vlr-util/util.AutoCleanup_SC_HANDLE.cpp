#include "pch.h"
#include "util.AutoCleanup_SC_HANDLE.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

HRESULT CAutoCleanup_SC_HANDLE::DoCleanup()
{
	if (m_hSCM == nullptr)
	{
		return S_FALSE;
	}

	auto bSuccess = ::CloseServiceHandle( m_hSCM );
	if (!bSuccess)
	{
		logging::LogMessagePF( VLR_LOG_CONTEXT_WARNING,
			_T( "OS call '%s' failed; error: %s" ),
			_T( "CloseServiceHandle" ),
			util::CResult::For_win32_LastError().ToString() );
	}

	return S_OK;
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
