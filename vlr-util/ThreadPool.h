#pragma once

#include "config.h"
#if VLR_HASDEP_BOOST_ASIO

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/use_future.hpp>

#include "cpp_namespace.h"

// Note: Some conceptual ideas derived from blog: https://dens.website/articles/cpp-threadpool

namespace vlr {

class CThreadPool
{
protected:
	std::mutex m_oMutex_ThreadPoolManip;
	cpp::shared_ptr<boost::asio::thread_pool> m_spThreadPool;
	std::optional<size_t> m_onFixedPoolSize;

public:
	decltype(auto) withDefaultPoolSize()
	{
		m_onFixedPoolSize = {};
		return *this;
	}
	decltype(auto) withFixedPoolSize( size_t nSize )
	{
		m_onFixedPoolSize = nSize;
		return *this;
	}

protected:
	HRESULT EnsurePoolInitialized();

public:
	template< typename F >
	HRESULT AddTaskToPool_ResultIgnored(F&& fTask);
	template< typename F >
	HRESULT AddTaskToPool_ResultAsFuture(F&& fTask, std::future<decltype(fTask())>& oFuture);
	template< typename F >
	auto AddTaskToPool_ResultAsFuture_Inline(F&& fTask)
	{
		std::future<decltype(fTask())> oFuture;
		AddTaskToPool_ResultAsFuture(std::forward<F>(fTask), oFuture);
		return oFuture;
	}

public:
	HRESULT WaitForCompletion();

public:
	~CThreadPool()
	{
		WaitForCompletion();
	}
};

} // namespace vlr

#include <future>

#include "UtilMacros.Assertions.h"

namespace vlr {

template< typename F >
HRESULT CThreadPool::AddTaskToPool_ResultIgnored(F&& fTask)
{
	HRESULT hr;

	hr = EnsurePoolInitialized();
	VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_HRESULT(hr);

	boost::asio::post(*m_spThreadPool, std::forward<F>(fTask));

	return S_OK;
}

template< typename F >
HRESULT CThreadPool::AddTaskToPool_ResultAsFuture(F&& fTask, std::future<decltype(fTask())>& oFuture)
{
	HRESULT hr;

	hr = EnsurePoolInitialized();
	VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_HRESULT(hr);

	std::promise<decltype(fTask())> oTaskExecResult;
	oFuture = oTaskExecResult.get_future();
	auto fExecTaskAndPopulatePromise = [oTaskExecResult = std::move(oTaskExecResult), fTask = std::forward<F>(fTask)]() mutable
	{
		oTaskExecResult.set_value(fTask());
	};
	boost::asio::post(*m_spThreadPool, std::move(fExecTaskAndPopulatePromise));

	return S_OK;
}

} // namespace vlr

#endif // VLR_HASDEP_BOOST_ASIO
