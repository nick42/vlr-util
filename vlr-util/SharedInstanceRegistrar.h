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
	static CSharedInstanceRegistrar& GetSharedInstance();

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

} // namespace vlr
