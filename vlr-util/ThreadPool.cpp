#include "pch.h"
#include "ThreadPool.h"

#if VLR_HASDEP_BOOST_ASIO

#include "UtilMacros.Assertions.h"

namespace vlr {

HRESULT CThreadPool::EnsurePoolInitialized()
{
	auto oLock_ThreadPoolManip = std::scoped_lock{ m_oMutex_ThreadPoolManip };

	if (m_spThreadPool)
	{
		return S_OK;
	}

	if (m_onFixedPoolSize.has_value())
	{
		m_spThreadPool = cpp::make_shared<boost::asio::thread_pool>( m_onFixedPoolSize.value() );
	}
	else
	{
		m_spThreadPool = cpp::make_shared<boost::asio::thread_pool>();
	}
	VLR_ASSERT_ALLOCATED_OR_RETURN_STANDARD_ERROR( m_spThreadPool );

	return S_OK;
}

HRESULT CThreadPool::WaitForCompletion()
{
	if (!m_spThreadPool)
	{
		return S_FALSE;
	}

	m_spThreadPool->wait();

	return S_OK;
}

} // namespace vlr

#endif VLR_HASDEP_BOOST_ASIO
