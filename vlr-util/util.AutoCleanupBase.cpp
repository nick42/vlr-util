#include "pch.h"
#include "util.AutoCleanupBase.h"

namespace vlr {

namespace util {

HRESULT CAutoCleanupBase::OnDestroy_DoCleanup()
{
	m_bDoCleanupCalled = true;

	if (!m_bDoCleanup)
	{
		return S_FALSE;
	}

	return DoCleanup();
}

} // namespace util

} // namespace vlr
