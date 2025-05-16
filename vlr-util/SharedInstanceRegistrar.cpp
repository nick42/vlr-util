#include "pch.h"
#include "SharedInstanceRegistrar.h"

namespace vlr {

CSharedInstanceRegistrar& CSharedInstanceRegistrar::GetSharedInstance()
{
	static auto theInstance = CSharedInstanceRegistrar{};
	return theInstance;
}

SResult CSharedInstanceRegistrar::SetSharedInstance(
	tzstring_view svzSharedInstanceName,
	const SPCSharedInstanceBase& spSharedInstanceBase)
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	m_mapInstanceNameToInstance[svzSharedInstanceName.toStdString()] = spSharedInstanceBase;

	return SResult::Success;
}

SResult CSharedInstanceRegistrar::ClearSharedInstance(
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

SResult CSharedInstanceRegistrar::ClearAllSharedInstances()
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

SResult CSharedInstanceRegistrar::CheckSharedInstanceCached(
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
