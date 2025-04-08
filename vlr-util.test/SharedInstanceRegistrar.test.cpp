#include "pch.h"

#include "vlr-util/SharedInstanceRegistrar.h"

using namespace vlr;

struct TestSharedInstanceRegistrar
	: public testing::Test
{
	CSharedInstanceRegistrar m_oSharedInstanceRegistrar;
};

class CTestSharedInstanceType
	: public CSharedInstanceBase
{
public:
	static tzstring_view GetTypeName()
	{
		return _T("CTestSharedInstanceType");
	}
	virtual tzstring_view GetSharedInstanceName() const
	{
		return GetTypeName();
	}
};

TEST_F(TestSharedInstanceRegistrar, PopulateSharedInstance_basic)
{
	SResult sr;

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);
}

TEST_F(TestSharedInstanceRegistrar, PopulateSharedInstance_ActuallyShared)
{
	SResult sr;

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance_SecondRequest;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance_SecondRequest);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance_SecondRequest, nullptr);

	// This should be the same
	EXPECT_EQ(spSharedInstance.get(), spSharedInstance_SecondRequest.get());
}

TEST_F(TestSharedInstanceRegistrar, PopulateSharedInstance_ExplicitOverride)
{
	SResult sr;

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance_Override = std::shared_ptr<CTestSharedInstanceType>{ new CTestSharedInstanceType };
	EXPECT_NE(spSharedInstance.get(), spSharedInstance_Override.get());

	sr = m_oSharedInstanceRegistrar.SetSharedInstance(CTestSharedInstanceType::GetTypeName(), spSharedInstance_Override);
	EXPECT_EQ(sr, SResult::Success);

	// Update to new instance
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);

	// This should be the same
	EXPECT_EQ(spSharedInstance.get(), spSharedInstance_Override.get());
}

TEST_F(TestSharedInstanceRegistrar, PopulateSharedInstance_IndirectPopulation)
{
	SResult sr;

	auto spSharedInstance = CTestSharedInstanceType::GetSharedInstanceMutableSP<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), &m_oSharedInstanceRegistrar);
	EXPECT_NE(spSharedInstance, nullptr);

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance_SecondRequest;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance_SecondRequest);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance_SecondRequest, nullptr);

	// This should be the same
	EXPECT_EQ(spSharedInstance.get(), spSharedInstance_SecondRequest.get());
}

TEST_F(TestSharedInstanceRegistrar, ClearSharedInstance)
{
	SResult sr;

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);

	m_oSharedInstanceRegistrar.ClearSharedInstance(CTestSharedInstanceType::GetTypeName());

	SPCSharedInstanceBase spSharedInstance_Check;
	sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceType::GetTypeName(), spSharedInstance_Check);
	EXPECT_TRUE(sr.isSuccess());
	EXPECT_EQ(spSharedInstance_Check, nullptr);
}

TEST_F(TestSharedInstanceRegistrar, ClearAllSharedInstances)
{
	SResult sr;

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);

	m_oSharedInstanceRegistrar.ClearAllSharedInstances();

	SPCSharedInstanceBase spSharedInstance_Check;
	sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceType::GetTypeName(), spSharedInstance_Check);
	EXPECT_TRUE(sr.isSuccess());
	EXPECT_EQ(spSharedInstance_Check, nullptr);
}

TEST_F(TestSharedInstanceRegistrar, RePopulateAfterClear)
{
	SResult sr;

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance, nullptr);

	m_oSharedInstanceRegistrar.ClearSharedInstance(CTestSharedInstanceType::GetTypeName());

	std::shared_ptr<CTestSharedInstanceType> spSharedInstance_SecondRequest;
	sr = m_oSharedInstanceRegistrar.PopulateSharedInstance<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), spSharedInstance_SecondRequest);
	EXPECT_EQ(sr, SResult::Success);
	EXPECT_NE(spSharedInstance_SecondRequest, nullptr);

	EXPECT_NE(spSharedInstance.get(), spSharedInstance_SecondRequest.get());
}
