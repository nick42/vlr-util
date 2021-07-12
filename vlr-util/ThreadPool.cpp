#include "pch.h"
#include "ThreadPool.h"

#include "UtilMacros.Assertions.h"

VLR_NAMESPACE_BEGIN( vlr )

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
	VLR_ASSERT_ALLOCATED__OR_RETURN_STANDARD_ERROR( m_spThreadPool );

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

VLR_NAMESPACE_END //( vlr )
