#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>

#include "zstring_view.h"
#include "util.Result.h"

namespace vlr {

class CSharedInstanceRegistrar;

class CSharedInstanceBase
{
public:
	virtual tzstring_view GetSharedInstanceName() const = 0;

	// Note: Intent here is for the child class to potentially add a static method which calls this, with name/template auto populated
	template <typename TSharedInstance>
	static std::shared_ptr<TSharedInstance> GetSharedInstanceMutableSP(
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

};

template <typename TSharedInstance>
std::shared_ptr<TSharedInstance> CSharedInstanceBase::GetSharedInstanceMutableSP(tzstring_view svzSharedInstanceName, CSharedInstanceRegistrar* pSharedInstanceRegistrar /*= nullptr*/)
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

	// Note: Creating with new explicitly, because we may take weak_ptr's
	// Note: This will not compile without a default constuctor (set manually otherwise)
	auto spSharedInstanceTyped = std::shared_ptr<TSharedInstance>{ new TSharedInstance() };
	if (!spSharedInstanceTyped)
	{
		return SResult::Failure;
	}

	spSharedInstanceBase = spSharedInstanceTyped;
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
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	m_mapInstanceNameToInstance.erase(svzSharedInstanceName.toStdString());

	return SResult::Success;
}

inline SResult CSharedInstanceRegistrar::ClearAllSharedInstances()
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	m_mapInstanceNameToInstance.clear();

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

} // namespace vlr
