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
	using TThisType = CTestSharedInstanceType;

	static tzstring_view GetTypeName()
	{
		return _T("CTestSharedInstanceType");
	}
	virtual tzstring_view GetSharedInstanceName() const
	{
		return GetTypeName();
	}
	static auto GetSharedInstanceMutableSP(CSharedInstanceRegistrar* pSharedInstanceRegistrar_Override = nullptr)
	{
		CSharedInstanceRegistrar* pSharedInstanceRegistrar = pSharedInstanceRegistrar_Override ?
			pSharedInstanceRegistrar_Override
			: &CSharedInstanceRegistrar::GetSharedInstance();
		std::shared_ptr<TThisType> spSharedInstance;
		pSharedInstanceRegistrar->PopulateSharedInstance<TThisType>(GetTypeName(), spSharedInstance);
		return spSharedInstance;
	}
};

class CTestSharedInstanceCreatedOnDestroy
	: public CSharedInstanceBase
{
public:
	using TThisType = CTestSharedInstanceCreatedOnDestroy;

	static tzstring_view GetTypeName()
	{
		return _T("CTestSharedInstanceCreatedOnDestroy");
	}
	virtual tzstring_view GetSharedInstanceName() const
	{
		return GetTypeName();
	}
	static auto GetSharedInstanceMutableSP(CSharedInstanceRegistrar* pSharedInstanceRegistrar_Override = nullptr)
	{
		CSharedInstanceRegistrar* pSharedInstanceRegistrar = pSharedInstanceRegistrar_Override ?
			pSharedInstanceRegistrar_Override
			: &CSharedInstanceRegistrar::GetSharedInstance();
		std::shared_ptr<TThisType> spSharedInstance;
		pSharedInstanceRegistrar->PopulateSharedInstance<TThisType>(GetTypeName(), spSharedInstance);
		return spSharedInstance;
	}
};


class CTestSharedInstanceWithActionOnDestroy
	: public CSharedInstanceBase
{
protected:
	CSharedInstanceRegistrar* m_pSharedInstanceRegistrar{};

public:
	using TThisType = CTestSharedInstanceWithActionOnDestroy;

	static tzstring_view GetTypeName()
	{
		return _T("CTestSharedInstanceWithActionOnDestroy");
	}
	virtual tzstring_view GetSharedInstanceName() const
	{
		return GetTypeName();
	}
	static auto GetSharedInstanceMutableSP(CSharedInstanceRegistrar* pSharedInstanceRegistrar_Override = nullptr)
	{
		CSharedInstanceRegistrar* pSharedInstanceRegistrar = pSharedInstanceRegistrar_Override ?
			pSharedInstanceRegistrar_Override
			: &CSharedInstanceRegistrar::GetSharedInstance();
		std::shared_ptr<TThisType> spSharedInstance;
		pSharedInstanceRegistrar->PopulateSharedInstance<TThisType>(GetTypeName(), spSharedInstance);
		return spSharedInstance;
	}

public:
	void SetSharedInstanceRegistrar(CSharedInstanceRegistrar* pSharedInstanceRegistrar_Override)
	{
		m_pSharedInstanceRegistrar = pSharedInstanceRegistrar_Override;
	}

	~CTestSharedInstanceWithActionOnDestroy()
	{
		CSharedInstanceRegistrar* pSharedInstanceRegistrar = m_pSharedInstanceRegistrar ?
			m_pSharedInstanceRegistrar
			: &CSharedInstanceRegistrar::GetSharedInstance();
		std::shared_ptr<CTestSharedInstanceCreatedOnDestroy> spSharedInstance;
		pSharedInstanceRegistrar->PopulateSharedInstance<CTestSharedInstanceCreatedOnDestroy>(CTestSharedInstanceCreatedOnDestroy::GetTypeName(), spSharedInstance);
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

	auto spSharedInstance = CTestSharedInstanceType::GetTypedSharedInstanceMutableSP<CTestSharedInstanceType>(CTestSharedInstanceType::GetTypeName(), &m_oSharedInstanceRegistrar);
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

TEST_F(TestSharedInstanceRegistrar, PopulateUsingClassDirectly)
{
	SResult sr;

	auto spSharedInstance = CTestSharedInstanceType::GetSharedInstanceMutableSP(&m_oSharedInstanceRegistrar);
	EXPECT_NE(spSharedInstance, nullptr);

	SPCSharedInstanceBase spSharedInstance_Check;
	sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceType::GetTypeName(), spSharedInstance_Check);
	EXPECT_TRUE(sr.isSuccess());
	EXPECT_NE(spSharedInstance_Check, nullptr);

	// This should be the same
	EXPECT_EQ(spSharedInstance.get(), spSharedInstance_Check.get());
}

TEST_F(TestSharedInstanceRegistrar, ClearSharedInstance_NoBlocking)
{
	SResult sr;

	auto spSharedInstance_WithActionOnDestroy = CTestSharedInstanceWithActionOnDestroy::GetSharedInstanceMutableSP(&m_oSharedInstanceRegistrar);
	EXPECT_NE(spSharedInstance_WithActionOnDestroy, nullptr);

	spSharedInstance_WithActionOnDestroy->SetSharedInstanceRegistrar(&m_oSharedInstanceRegistrar);

	spSharedInstance_WithActionOnDestroy = {};

	// Note: Should still be in the registrar
	{
		SPCSharedInstanceBase spSharedInstance;
		sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceWithActionOnDestroy::GetTypeName(), spSharedInstance);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_NE(spSharedInstance, nullptr);
	}
	// .. but other class not created yet
	{
		SPCSharedInstanceBase spSharedInstance;
		sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceCreatedOnDestroy::GetTypeName(), spSharedInstance);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(spSharedInstance, nullptr);
	}
	EXPECT_EQ(m_oSharedInstanceRegistrar.GetSharedInstanceCount(), 1);

	// This should clear the one shared instance, and create the other (via the destructor), and be non-blocking

	sr = m_oSharedInstanceRegistrar.ClearSharedInstance(CTestSharedInstanceWithActionOnDestroy::GetTypeName());
	EXPECT_TRUE(sr.isSuccess());

	// Should not be in the registrar
	{
		SPCSharedInstanceBase spSharedInstance;
		sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceWithActionOnDestroy::GetTypeName(), spSharedInstance);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_EQ(spSharedInstance, nullptr);
	}
	// .. but other class should have been created and added
	{
		SPCSharedInstanceBase spSharedInstance;
		sr = m_oSharedInstanceRegistrar.CheckSharedInstanceCached(CTestSharedInstanceCreatedOnDestroy::GetTypeName(), spSharedInstance);
		EXPECT_TRUE(sr.isSuccess());
		EXPECT_NE(spSharedInstance, nullptr);
	}
	EXPECT_EQ(m_oSharedInstanceRegistrar.GetSharedInstanceCount(), 1);
}

TEST_F(TestSharedInstanceRegistrar, ClearAllSharedInstances_IncludingIndirectCreated)
{
	SResult sr;

	auto spSharedInstance_WithActionOnDestroy = CTestSharedInstanceWithActionOnDestroy::GetSharedInstanceMutableSP(&m_oSharedInstanceRegistrar);
	EXPECT_NE(spSharedInstance_WithActionOnDestroy, nullptr);

	spSharedInstance_WithActionOnDestroy->SetSharedInstanceRegistrar(&m_oSharedInstanceRegistrar);

	spSharedInstance_WithActionOnDestroy = {};
	EXPECT_EQ(m_oSharedInstanceRegistrar.GetSharedInstanceCount(), 1);

	// This should clear the instance there, and the triggered created instance (as it iterates until the list is empty)

	sr = m_oSharedInstanceRegistrar.ClearAllSharedInstances();
	EXPECT_TRUE(sr.isSuccess());

	EXPECT_EQ(m_oSharedInstanceRegistrar.GetSharedInstanceCount(), 0);
}
