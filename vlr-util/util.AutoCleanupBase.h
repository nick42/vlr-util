#pragma once

namespace vlr {

namespace util {

class CAutoCleanupBase
{
public:
	bool m_bDoCleanup = true;
	bool m_bDoCleanupCalled = false;

protected:
	// Note: Implement this method in child class to perform the resource cleanup
	virtual HRESULT DoCleanup() = 0;

protected:
	// Call this from the child class destructor
	HRESULT OnDestroy_DoCleanup();

public:
	virtual ~CAutoCleanupBase()
	{
		ASSERT( m_bDoCleanupCalled );
	}
};

} // namespace util

} // namespace vlr
