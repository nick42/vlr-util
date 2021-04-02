#include "pch.h"
#include "util.AutoCleanupBase.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

HRESULT CAutoCleanupBase::OnDestroy_DoCleanup()
{
	m_bDoCleanupCalled = true;

	if (!m_bDoCleanup)
	{
		return S_FALSE;
	}

	return DoCleanup();
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
