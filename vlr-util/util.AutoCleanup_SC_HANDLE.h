#pragma once

#include "util.AutoCleanupBase.h"
#include "cpp_namespace.h"

#include "logging.LogMessage.h"
#include "util.Result.h"

namespace vlr {

namespace util {

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

} // namespace util

} // namespace vlr
