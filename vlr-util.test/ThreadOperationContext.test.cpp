#include "pch.h"

#include <gtest/gtest.h>

#include <condition_variable>
#include <mutex>

#include "vlr-util/ThreadOperationContext.h"
#include "vlr-util/StringCompare.h"

using namespace vlr;

struct TestThreadOperationContext
	: public testing::Test
{
};

static constexpr vlr::tzstring_view svzContextValue_Module_Name = _T("Module");
static constexpr vlr::tzstring_view svzContextValue_Module_Value = _T("UnitTest");
static constexpr vlr::tzstring_view svzContextValue_SubModule_Name = _T("SomeTest");
static constexpr vlr::tzstring_view svzContextValue_SubModule_Value = _T("TestName");

TEST_F(TestThreadOperationContext, basic)
{
	SResult sr;

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}

	auto spContextLifetime_Module = ThreadOperationContext::AddOperationContext(svzContextValue_Module_Name, svzContextValue_Module_Value);
	ASSERT_NE(spContextLifetime_Module, nullptr);
	ASSERT_NE(spContextLifetime_Module->m_spContext, nullptr);
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sName, svzContextValue_Module_Name));
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sValue, svzContextValue_Module_Value));

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 1);

		const auto& spContext = vecContextStack.back();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_Module_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_Module_Value));
	}

	spContextLifetime_Module = {};

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}
}

TEST_F(TestThreadOperationContext, MultiLevelContext)
{
	SResult sr;

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}

	auto spContextLifetime_Module = ThreadOperationContext::AddOperationContext(svzContextValue_Module_Name, svzContextValue_Module_Value);
	ASSERT_NE(spContextLifetime_Module, nullptr);
	ASSERT_NE(spContextLifetime_Module->m_spContext, nullptr);
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sName, svzContextValue_Module_Name));
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sValue, svzContextValue_Module_Value));

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 1);

		const auto& spContext = vecContextStack.front();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_Module_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_Module_Value));
	}

	auto spContextLifetime_SubModule = ThreadOperationContext::AddOperationContext(svzContextValue_SubModule_Name, svzContextValue_SubModule_Value);
	ASSERT_NE(spContextLifetime_SubModule, nullptr);
	ASSERT_NE(spContextLifetime_SubModule->m_spContext, nullptr);
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_SubModule->m_spContext->m_sName, svzContextValue_SubModule_Name));
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_SubModule->m_spContext->m_sValue, svzContextValue_SubModule_Value));

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 2);

		const auto& spContext = vecContextStack.back();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_SubModule_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_SubModule_Value));
	}

	spContextLifetime_SubModule = {};

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 1);

		const auto& spContext = vecContextStack.back();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_Module_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_Module_Value));
	}

	spContextLifetime_Module = {};

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}
}

TEST_F(TestThreadOperationContext, PopContext_Invalid)
{
	SResult sr;

	sr = CThreadOperationContext::GetSharedInstance().PopContext(_T("Invalid"));
	EXPECT_TRUE(sr.isSuccess());

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}
}

TEST_F(TestThreadOperationContext, OutOfOrderContextPop)
{
	SResult sr;

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}

	auto spContextLifetime_Module = ThreadOperationContext::AddOperationContext(svzContextValue_Module_Name, svzContextValue_Module_Value);
	ASSERT_NE(spContextLifetime_Module, nullptr);
	ASSERT_NE(spContextLifetime_Module->m_spContext, nullptr);
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sName, svzContextValue_Module_Name));
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sValue, svzContextValue_Module_Value));

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 1);

		const auto& spContext = vecContextStack.front();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_Module_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_Module_Value));
	}

	auto spContextLifetime_SubModule = ThreadOperationContext::AddOperationContext(svzContextValue_SubModule_Name, svzContextValue_SubModule_Value);
	ASSERT_NE(spContextLifetime_SubModule, nullptr);
	ASSERT_NE(spContextLifetime_SubModule->m_spContext, nullptr);
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_SubModule->m_spContext->m_sName, svzContextValue_SubModule_Name));
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_SubModule->m_spContext->m_sValue, svzContextValue_SubModule_Value));

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 2);

		const auto& spContext = vecContextStack.back();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_SubModule_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_SubModule_Value));
	}

	spContextLifetime_Module = {};

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 1);

		const auto& spContext = vecContextStack.back();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_SubModule_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_SubModule_Value));
	}

	spContextLifetime_SubModule = {};

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}
}

TEST_F(TestThreadOperationContext, basic_MultiThread)
{
	// Basically, ensure that context in another thread does not interfere with the context of the main thread

	SResult sr;

	bool bOtherThreadIsReady = false;
	bool bMainThreadIsComplete = false;
	std::mutex mutexThreadComms;
	std::condition_variable cvThreadStateUpdated;

	auto threadOther = std::thread{ [&]() {
		auto spContextLifetime_Other = ThreadOperationContext::AddOperationContext(svzContextValue_Module_Name, _T("OtherModule"));

		{
			auto slThreadComms = std::unique_lock{ mutexThreadComms };
			bOtherThreadIsReady = true;
		}
		cvThreadStateUpdated.notify_all();

		{
			auto slThreadComms = std::unique_lock{ mutexThreadComms };
			cvThreadStateUpdated.wait(slThreadComms, [&] { return bMainThreadIsComplete; });
		}
	} };

	{
		auto slThreadComms = std::unique_lock{ mutexThreadComms };
		cvThreadStateUpdated.wait(slThreadComms, [&] { return bOtherThreadIsReady; });
	}

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}

	auto spContextLifetime_Module = ThreadOperationContext::AddOperationContext(svzContextValue_Module_Name, svzContextValue_Module_Value);
	ASSERT_NE(spContextLifetime_Module, nullptr);
	ASSERT_NE(spContextLifetime_Module->m_spContext, nullptr);
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sName, svzContextValue_Module_Name));
	EXPECT_TRUE(StringCompare::CS().AreEqual(spContextLifetime_Module->m_spContext->m_sValue, svzContextValue_Module_Value));

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_EQ(sr, S_OK);
		ASSERT_EQ(vecContextStack.size(), 1);

		const auto& spContext = vecContextStack.back();
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sName, svzContextValue_Module_Name));
		EXPECT_TRUE(StringCompare::CS().AreEqual(spContext->m_sValue, svzContextValue_Module_Value));
	}

	spContextLifetime_Module = {};

	{
		std::vector<ThreadOperationContext::SPCGenericContext> vecContextStack;
		sr = CThreadOperationContext::GetSharedInstance().PopulateThreadOperationContext(vecContextStack);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(vecContextStack.size(), 0);
	}

	{
		auto slThreadComms = std::unique_lock{ mutexThreadComms };
		bMainThreadIsComplete = true;
	}
	cvThreadStateUpdated.notify_all();

	threadOther.join();
}
