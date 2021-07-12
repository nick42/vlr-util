#include "pch.h"
#include "util.AutoCleanupBase.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

HRESULT CAutoCleanupBase::OnDestroy_DoCleanup()
{
	m_bDoCleanupCalled = true;

	if (!m_bDoCleanup)
	{
		return S_FALSE;
	}

	return DoCleanup();
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
