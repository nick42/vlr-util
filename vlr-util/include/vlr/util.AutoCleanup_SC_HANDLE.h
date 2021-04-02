#pragma once

#include "vlr/util.AutoCleanupBase.h"
#include "vlr/cpp_namespace.h"

#include "vlr/logging.LogMessage.h"
#include "vlr/util.Result.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

class CAutoCleanup_SC_HANDLE
	: public CAutoCleanupBase
{
public:
	SC_HANDLE m_hSCM = nullptr;

protected:
	virtual HRESULT DoCleanup();

public:
	CAutoCleanup_SC_HANDLE( SC_HANDLE hSCM )
		: m_hSCM{ hSCM }
	{}
	// Should not copy; would break cleanup
	CAutoCleanup_SC_HANDLE( const CAutoCleanup_SC_HANDLE& ) = delete;
	// Move is okey though
	CAutoCleanup_SC_HANDLE( CAutoCleanup_SC_HANDLE&& ) = default;
	virtual ~CAutoCleanup_SC_HANDLE()
	{
		OnDestroy_DoCleanup();
	}
};

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
