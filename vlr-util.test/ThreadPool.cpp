#include "pch.h"

#include "vlr-util/ThreadPool.h"

TEST(ThreadPool, general)
{
	auto oThreadPool = vlr::CThreadPool{};

	LONG nValue = 0;

	for (auto i = 0; i < 100; ++i)
	{
		oThreadPool.AddTaskToPool_ResultIgnored( [pValue = &nValue]() { InterlockedAdd( pValue, 1 ); } );
	}
	oThreadPool.WaitForCompletion();

	ASSERT_EQ( nValue, 100 );
}

TEST(ThreadPool, ResultAsFuture)
{
	auto oThreadPool = vlr::CThreadPool{};

	auto oFutureAnswer = oThreadPool.AddTaskToPool_ResultAsFuture_Inline( [] { return 42; } );

	auto nFutureAnswer = oFutureAnswer.get();
	ASSERT_EQ( nFutureAnswer, 42 );
}
