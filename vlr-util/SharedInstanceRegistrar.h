#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

#include "StringCompare.h"
#include "zstring_view.h"
#include "util.Result.h"
#include "ModuleContext.LogicalState.h"

namespace vlr {

class CSharedInstanceRegistrar;

class CSharedInstanceBase
{
public:
	virtual tzstring_view GetSharedInstanceName() const = 0;

	// Note: Intent here is for the child class to potentially add a static method which calls this, with name/template auto populated
	template <typename TSharedInstance>
	static std::shared_ptr<TSharedInstance> GetTypedSharedInstanceMutableSP(
		tzstring_view svzSharedInstanceName,
		CSharedInstanceRegistrar* pSharedInstanceRegistrar = nullptr);

public:
	virtual ~CSharedInstanceBase() = default;
};
using SPCSharedInstanceBase = std::shared_ptr<CSharedInstanceBase>;
using WPCSharedInstanceBase = std::weak_ptr<CSharedInstanceBase>;

class CSharedInstanceRegistrar
{
public:
	// #irony...
	// (we cannot use the registrar for itself, and this cannot be a global)
	static auto& GetSharedInstance()
	{
		static auto theInstance = CSharedInstanceRegistrar{};
		return theInstance;
	}

protected:
	mutable std::mutex m_mutexDataAccess;
	std::unordered_map<tstring, SPCSharedInstanceBase> m_mapInstanceNameToInstance;
	// Note: We maintain an ordered vector of instances, so that we can destruct in reverse order
	std::vector<std::tuple<tstring, SPCSharedInstanceBase>> m_vecSharedInstanceCollection;

public:
	template <typename TSharedInstance>
	SResult PopulateSharedInstance(
		tzstring_view svzSharedInstanceName,
		std::shared_ptr<TSharedInstance>& spSharedInstance_Result);

	SResult SetSharedInstance(
		tzstring_view svzSharedInstanceName,
		const SPCSharedInstanceBase& spSharedInstanceBase);
	SResult ClearSharedInstance(
		tzstring_view svzSharedInstanceName);
	SResult ClearAllSharedInstances();
	// On cached, will populate; else will populate as nullptr, without modifying the cache
	SResult CheckSharedInstanceCached(
		tzstring_view svzSharedInstanceName,
		SPCSharedInstanceBase& spSharedInstance_Result) const;
	size_t GetSharedInstanceCount() const;

protected:
	SResult EnsureSharedInstanceInOrderedCollection_UnderLock(
		tzstring_view svzSharedInstanceName,
		const SPCSharedInstanceBase& spSharedInstanceBase);
	SResult RemoveSharedInstanceFromOrderedCollection_UnderLock(
		tzstring_view svzSharedInstanceName,
		SPCSharedInstanceBase& spSharedInstanceBase_Result);

};

template <typename TSharedInstance>
std::shared_ptr<TSharedInstance> CSharedInstanceBase::GetTypedSharedInstanceMutableSP(tzstring_view svzSharedInstanceName, CSharedInstanceRegistrar* pSharedInstanceRegistrar /*= nullptr*/)
{
	auto& oSharedInstanceRegistrar = pSharedInstanceRegistrar ? *pSharedInstanceRegistrar : CSharedInstanceRegistrar::GetSharedInstance();

	static auto wpSharedInstance = [&]() -> std::weak_ptr<TSharedInstance>
	{
		std::shared_ptr<TSharedInstance> spSharedInstance;
		oSharedInstanceRegistrar.PopulateSharedInstance<TSharedInstance>(svzSharedInstanceName, spSharedInstance);
		return spSharedInstance;
	}();

	auto spSharedInstance = wpSharedInstance.lock();
	if (spSharedInstance)
	{
		return spSharedInstance;
	}

	// Note: We try again here, because the instance in the registrar may have been updated
	std::shared_ptr<TSharedInstance> spSharedInstance_PossibleUpdate;
	oSharedInstanceRegistrar.PopulateSharedInstance<TSharedInstance>(svzSharedInstanceName, spSharedInstance_PossibleUpdate);

	// We store and return this result regardless of validity
	// Note: This corner-case is not necessarily thread safe (I'm unsure); unsure if this is worth the trade-off of having a mutex here
	wpSharedInstance = spSharedInstance_PossibleUpdate;

	return spSharedInstance_PossibleUpdate;
}

template <typename TSharedInstance>
SResult CSharedInstanceRegistrar::PopulateSharedInstance(
	tzstring_view svzSharedInstanceName,
	std::shared_ptr<TSharedInstance>& spSharedInstance_Result)
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	auto& spSharedInstanceBase = m_mapInstanceNameToInstance[svzSharedInstanceName.toStdString()];
	if (spSharedInstanceBase)
	{
		spSharedInstance_Result = std::dynamic_pointer_cast<TSharedInstance>(spSharedInstanceBase);
		if (!spSharedInstance_Result)
		{
			return SResult::Failure;
		}

		return SResult::Success;
	}

	bool bNormalStateForClassInit = ModuleContext::CLogicalState::GetSharedInstance().IsInNormalStateFor_ClassInit();
	// Note: Only doing debug check here; not safe to log or take other actions.
	ASSERT(bNormalStateForClassInit);

	// Note: Creating with new explicitly, because we may take weak_ptr's
	// Note: This will not compile without a default constuctor (set manually otherwise)
	auto spSharedInstanceTyped = std::shared_ptr<TSharedInstance>{ new TSharedInstance() };
	if (!spSharedInstanceTyped)
	{
		return SResult::Failure;
	}

	spSharedInstanceBase = spSharedInstanceTyped;
	m_vecSharedInstanceCollection.push_back(std::make_tuple(svzSharedInstanceName.toStdString(), spSharedInstanceBase));
	spSharedInstance_Result = spSharedInstanceTyped;

	return SResult::Success;
}

inline SResult CSharedInstanceRegistrar::SetSharedInstance(
	tzstring_view svzSharedInstanceName,
	const SPCSharedInstanceBase& spSharedInstanceBase)
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	m_mapInstanceNameToInstance[svzSharedInstanceName.toStdString()] = spSharedInstanceBase;

	return SResult::Success;
}

inline SResult CSharedInstanceRegistrar::ClearSharedInstance(
	tzstring_view svzSharedInstanceName)
{
	// Note: We're being a bit careful here. It's _possible_ that the destructor for a shared instance 
	// will invoke other functionality in the registrar. If we are holding the mutex at that time, we 
	// would deadlock. So we release the internal mutex _before_ we free our handle to the previous 
	// shared instance.

	SPCSharedInstanceBase spSharedInstanceBase_Removing;

	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		m_mapInstanceNameToInstance.erase(svzSharedInstanceName.toStdString());
		RemoveSharedInstanceFromOrderedCollection_UnderLock(svzSharedInstanceName, spSharedInstanceBase_Removing);
	}

	return SResult::Success;
}

inline SResult CSharedInstanceRegistrar::ClearAllSharedInstances()
{
	// Note: See note in ClearSharedInstance for caution about reference holding.

	while (true)
	{
		SPCSharedInstanceBase spSharedInstanceBase_Removing;

		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		if (m_vecSharedInstanceCollection.size() == 0)
		{
			break;
		}
		spSharedInstanceBase_Removing = std::get<1>(m_vecSharedInstanceCollection.back());
		m_vecSharedInstanceCollection.pop_back();

		m_mapInstanceNameToInstance.erase(spSharedInstanceBase_Removing->GetSharedInstanceName().toStdString());
	}

	return SResult::Success;
}

inline SResult CSharedInstanceRegistrar::CheckSharedInstanceCached(
	tzstring_view svzSharedInstanceName,
	SPCSharedInstanceBase& spSharedInstance_Result) const
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	auto iterIndex = m_mapInstanceNameToInstance.find(svzSharedInstanceName.toStdString());
	if (iterIndex == m_mapInstanceNameToInstance.end())
	{
		spSharedInstance_Result = {};
		return SResult::Success_WithNuance;
	}

	spSharedInstance_Result = iterIndex->second;

	return SResult::Success;
}

size_t CSharedInstanceRegistrar::GetSharedInstanceCount() const
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	return m_mapInstanceNameToInstance.size();
}

SResult CSharedInstanceRegistrar::EnsureSharedInstanceInOrderedCollection_UnderLock(
	tzstring_view svzSharedInstanceName,
	const SPCSharedInstanceBase& spSharedInstanceBase)
{
	for (auto& oSharedInstanceTuple : m_vecSharedInstanceCollection)
	{
		if (vlr::StringCompare::CS().AreEqual(std::get<0>(oSharedInstanceTuple), svzSharedInstanceName))
		{
			if (std::get<1>(oSharedInstanceTuple).get() == spSharedInstanceBase.get())
			{
				return SResult::Success;
			}
			// Update with new shared instance (eg: from set operation)
			std::get<1>(oSharedInstanceTuple) = spSharedInstanceBase;

			return SResult::Success;
		}
	}

	// If we get to here, the instance was not in the ordered collection previously; add to the end
	m_vecSharedInstanceCollection.push_back(std::make_tuple(svzSharedInstanceName.toStdString(), spSharedInstanceBase));

	return SResult::Success;
}

SResult CSharedInstanceRegistrar::RemoveSharedInstanceFromOrderedCollection_UnderLock(
	tzstring_view svzSharedInstanceName,
	SPCSharedInstanceBase& spSharedInstanceBase_Result)
{
	for (auto iterIndex = m_vecSharedInstanceCollection.begin(); iterIndex != m_vecSharedInstanceCollection.end(); ++iterIndex)
	{
		const auto& oSharedInstanceTuple = *iterIndex;
		if (vlr::StringCompare::CS().AreEqual(std::get<0>(oSharedInstanceTuple), svzSharedInstanceName))
		{
			spSharedInstanceBase_Result = std::get<1>(oSharedInstanceTuple);
			m_vecSharedInstanceCollection.erase(iterIndex);
			// Note: Reference above not valid any more here, just to note.
			return SResult::Success;
		}
	}

	// If we get to here, the instance was not in the ordered collection previously

	return SResult::Success_WithNuance;
}

} // namespace vlr
