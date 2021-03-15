#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/use_future.hpp>

#include "cpp_namespace.h"

// Note: Some conceptual ideas derived from blog: https://dens.website/articles/cpp-threadpool

NAMESPACE_BEGIN( vlr )

class CThreadPool
{
protected:
	std::mutex m_oMutex_ThreadPoolManip;
	cpp::shared_ptr<boost::asio::thread_pool> m_spThreadPool;
	std::optional<size_t> m_onFixedPoolSize;

public:
	decltype(auto) With_DefaultPoolSize()
	{
		m_onFixedPoolSize = {};
		return *this;
	}
	decltype(auto) With_FixedPoolSize( size_t nSize )
	{
		m_onFixedPoolSize = nSize;
		return *this;
	}

protected:
	HRESULT EnsurePoolInitialized();

public:
	template< typename F >
	HRESULT AddTaskToPool_ResultIgnored( F&& fTask );
	template< typename F >
	HRESULT AddTaskToPool_ResultAsFuture( F&& fTask, std::future<decltype(fTask())>& oFuture );
	template< typename F >
	auto AddTaskToPool_ResultAsFuture_Inline( F&& fTask )
	{
		std::future<decltype(fTask())> oFuture;
		AddTaskToPool_ResultAsFuture( std::forward<F>( fTask ), oFuture );
		return oFuture;
	}

public:
	HRESULT WaitForCompletion();

};

NAMESPACE_END //( vlr )

#include <future>

#include "vlr/UtilMacros.Assertions.h"

NAMESPACE_BEGIN( vlr )

template< typename F >
HRESULT CThreadPool::AddTaskToPool_ResultIgnored( F&& fTask )
{
	HRESULT hr;

	hr = EnsurePoolInitialized();
	ASSERT_HR_SUCCEEDED__OR_RETURN_HRESULT( hr );

	boost::asio::post( *m_spThreadPool, std::forward<F>( fTask ) );

	return S_OK;
}

template< typename F >
HRESULT CThreadPool::AddTaskToPool_ResultAsFuture( F&& fTask, std::future<decltype(fTask())>& oFuture )
{
	HRESULT hr;

	hr = EnsurePoolInitialized();
	ASSERT_HR_SUCCEEDED__OR_RETURN_HRESULT( hr );

	std::promise<decltype(fTask())> oTaskExecResult;
	oFuture = oTaskExecResult.get_future();
	auto fExecTaskAndPopulatePromise = [oTaskExecResult = std::move( oTaskExecResult ), fTask = std::forward<F>( fTask )]() mutable
	{
		oTaskExecResult.set_value( fTask() );
	};
	boost::asio::post( *m_spThreadPool, std::move( fExecTaskAndPopulatePromise ) );

	return S_OK;
}

NAMESPACE_END //( vlr )
