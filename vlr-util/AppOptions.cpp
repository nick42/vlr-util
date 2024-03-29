#include "pch.h"
#include "AppOptions.h"

namespace vlr {

SResult CAppOptions::PopulateCollectionOfSpecifiedOptionValues_NameMatch(
	const vlr::tstring& sNormalizedName,
	std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues)
{
	SResult sr;

	for (const auto& oMapPair : m_mapSpecifiedNameToSpecifiedValue)
	{
		auto& spSpecifiedValue = oMapPair.second;
		VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spSpecifiedValue);

		sr = spSpecifiedValue->CheckForOptionMatch(sNormalizedName);
		if (sr != S_OK)
		{
			continue;
		}

		vecSpecifiedValues.push_back(spSpecifiedValue);
	}

	return S_OK;
}

SResult CAppOptions::ClearAllPrePopulatedSpecifiedValuesOnValueAdd(
	const vlr::tstring& sSpecifiedName)
{
	// When we add a new value, we should clear out any pre-populated options matching the name,
	// so that we can recompute the effective value on next reference.

	SResult sr;

	std::unordered_set<vlr::tstring> setOptionNamesToRemove;

	for (const auto& oMapPair : m_mapNormalizedNameToSpecifiedValue)
	{
		sr = CAppOptionSpecifiedValue::CheckOptionNameMatch(sSpecifiedName, oMapPair.first);
		if (sr != S_OK)
		{
			continue;
		}
		setOptionNamesToRemove.insert(oMapPair.first);
	}

	for (const auto& sName : setOptionNamesToRemove)
	{
		m_mapNormalizedNameToSpecifiedValue.erase(sName);
	}

	return S_OK;
}

SResult CAppOptions::AddSpecifiedValue(const SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue)
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spAppOptionSpecifiedValue);

	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	auto& spValueInMap = m_mapSpecifiedNameToSpecifiedValue[spAppOptionSpecifiedValue->GetNativeOptionName()];
	bool bHadPreviousValue = (spValueInMap != nullptr);
	spValueInMap = spAppOptionSpecifiedValue;

	ClearAllPrePopulatedSpecifiedValuesOnValueAdd(spAppOptionSpecifiedValue->GetNativeOptionName());

	return bHadPreviousValue ? SResult::Success_WithNuance : SResult::Success;
}

SResult CAppOptions::FindSpecifiedValueByName(const vlr::tstring& sSpecifiedName, SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue) const
{
	VLR_ASSERT_NOTBLANK_OR_RETURN_EUNEXPECTED(sSpecifiedName);

	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	auto iterIndex = m_mapSpecifiedNameToSpecifiedValue.find(sSpecifiedName);
	if (iterIndex == m_mapSpecifiedNameToSpecifiedValue.end())
	{
		return S_FALSE;
	}

	spAppOptionSpecifiedValue = iterIndex->second;

	return S_OK;
}

SResult CAppOptions::FindSpecifiedValuesMatchingNormalizedName(
	const vlr::tstring& sNormalizedName,
	std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues)
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	return PopulateCollectionOfSpecifiedOptionValues_NameMatch(sNormalizedName, vecSpecifiedValues);
}

SResult CAppOptions::PopulatePrePreparedSpecifiedValueForOption(
	const vlr::tstring& sNormalizedName,
	SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue) const
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	auto iterIndex = m_mapNormalizedNameToSpecifiedValue.find(sNormalizedName);
	if (iterIndex == m_mapNormalizedNameToSpecifiedValue.end())
	{
		return S_FALSE;
	}

	spAppOptionSpecifiedValue = iterIndex->second;

	return S_OK;
}

SResult CAppOptions::ClearPrePopulatedSpecifiedValueForOption(
	const vlr::tstring& sNormalizedName)
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	m_mapNormalizedNameToSpecifiedValue.erase(sNormalizedName);

	return S_OK;
}

SResult CAppOptions::SetAppOptionQualifiers(
	const vlr::tstring& sNormalizedName,
	const SPCAppOptionQualifiers& spAppOptionQualifiers)
{
	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	auto& spAppOptionQualifiers_InMap = m_mapNormalizedNameToQualifiers[sNormalizedName];
	bool bOverwriteExisting = (spAppOptionQualifiers_InMap != nullptr);
	spAppOptionQualifiers_InMap = spAppOptionQualifiers;

	// If we have accessed this app option, then set the qualifiers in that structure as well
	do
	{
		auto iterIndex = m_mapNormalizedNameToSpecifiedValue.find(sNormalizedName);
		if (iterIndex == m_mapNormalizedNameToSpecifiedValue.end())
		{
			break;
		}
		auto spSpecifiedValue = iterIndex->second;

		spSpecifiedValue->withAppOptionQualifiers(spAppOptionQualifiers);
	} while (false);

	// Note: We do not set qualifiers for specified values, until/unless they are accessed

	return S_OK;
}

} // namespace vlr
