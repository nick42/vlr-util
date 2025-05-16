#pragma once

#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "AppOptionQualifiers.h"
#include "AppOptionSpecifiedValue.h"

// Note: Methods of this class are thread-safe, for MT calls.

namespace vlr {

class CAppOptions
{
public:
	static CAppOptions& GetSharedInstance();

protected:
	mutable std::mutex m_mutexDataAccess;
	std::unordered_map<vlr::tstring, SPCAppOptionSpecifiedValue> m_mapSpecifiedNameToSpecifiedValue;

	// Note: We will populate nullptr values in this map for accessed values.
	// This will eliminate needing to keep a separate set of accessed values; can be implicit in this map.
	std::unordered_map<vlr::tstring, SPCAppOptionSpecifiedValue> m_mapNormalizedNameToSpecifiedValue;

	// This is the set of specified qualifiers, based on normalized option name.
	// This is set separately, so that qualifiers can be set before options are accessed and/or populated with specified values.
	std::unordered_map<vlr::tstring, SPCAppOptionQualifiers> m_mapNormalizedNameToQualifiers;

protected:
	// Note: For protected methods, they assume data access mutex is already acquired

	template <typename TValue>
	SResult HandleOptionInitialAccess(
		const vlr::tstring& sNormalizedName);
	SResult PopulateCollectionOfSpecifiedOptionValues_NameMatch(
		const vlr::tstring& sNormalizedName,
		std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues);
	template <typename TValue>
	SResult PopulateCollectionOfSpecifiedOptionValues(
		const vlr::tstring& sNormalizedName,
		std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues);
	SResult ClearAllPrePopulatedSpecifiedValuesOnValueAdd(
		const vlr::tstring& sSpecifiedName);

public:
	SResult AddSpecifiedValue(const SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue);

	inline auto GetCount_SpecifiedValues() const
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };
		return m_mapSpecifiedNameToSpecifiedValue.size();
	}
	inline auto GetCount_PrePreparedValues() const
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };
		return m_mapNormalizedNameToSpecifiedValue.size();
	}

	SResult FindSpecifiedValueByName(const vlr::tstring& sSpecifiedName, SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue) const;

	SResult FindSpecifiedValuesMatchingNormalizedName(
		const vlr::tstring& sNormalizedName,
		std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues);

	// This method does a couple things:
	// - Find the value if specified (and resolve any potential ambiguity in specification)
	// - Move the value to the collection for "accessed" options
	// - Pre-populate the TValue type in the specified value, for future access without conversion
	// Note: Returns S_FALSE if the value is not specified
	template <typename TValue>
	SResult PopulateSpecifiedValueForOption(
		const vlr::tstring& sNormalizedName,
		SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue);

	// Note: Only returns values which have been previously prepared
	SResult PopulatePrePreparedSpecifiedValueForOption(
		const vlr::tstring& sNormalizedName,
		SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue) const;

	SResult ClearPrePopulatedSpecifiedValueForOption(
		const vlr::tstring& sNormalizedName);

	SResult SetAppOptionQualifiers(
		const vlr::tstring& sNormalizedName,
		const SPCAppOptionQualifiers& spAppOptionQualifiers);

};

template <typename TValue>
SResult CAppOptions::HandleOptionInitialAccess(
	const vlr::tstring& sNormalizedName)
{
	SResult sr;

	std::vector<SPCAppOptionSpecifiedValue> vecSpecifiedValues;
	sr = PopulateCollectionOfSpecifiedOptionValues<TValue>(sNormalizedName, vecSpecifiedValues);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	if (vecSpecifiedValues.size() == 0)
	{
		m_mapNormalizedNameToSpecifiedValue[sNormalizedName] = nullptr;
		return S_OK;
	}

	// TODO: Allow configurability of preference if there are multiple set values
	// Placeholder for now...
	auto spSpecifiedValue = vecSpecifiedValues[0];

	m_mapNormalizedNameToSpecifiedValue[sNormalizedName] = spSpecifiedValue;

	// Add qualifiers to value, if specified
	do
	{
		auto iterIndex = m_mapNormalizedNameToQualifiers.find(sNormalizedName);
		if (iterIndex == m_mapNormalizedNameToQualifiers.end())
		{
			break;
		}
		auto& spQualifiers = iterIndex->second;

		spSpecifiedValue->withAppOptionQualifiers(spQualifiers);
	} while (false);

	return S_OK;
}

template <typename TValue>
SResult CAppOptions::PopulateCollectionOfSpecifiedOptionValues(
	const vlr::tstring& sNormalizedName,
	std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues)
{
	SResult sr;

	std::vector<SPCAppOptionSpecifiedValue> vecSpecifiedValues_NameMatch;
	sr = PopulateCollectionOfSpecifiedOptionValues_NameMatch(sNormalizedName, vecSpecifiedValues_NameMatch);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	for (const auto& spSpecifiedValue : vecSpecifiedValues_NameMatch)
	{
		sr = spSpecifiedValue->CacheOptionValueAs<TValue>();
		if (sr != S_OK)
		{
			// Could not convert specified option value to requested value; TODO: log this
			continue;
		}

		vecSpecifiedValues.push_back(spSpecifiedValue);
	}

	return S_OK;
}

template <typename TValue>
SResult CAppOptions::PopulateSpecifiedValueForOption(
	const vlr::tstring& sNormalizedName,
	SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue)
{
	SResult sr;

	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	{
		auto iterIndex_MapToSpecifiedValue = m_mapNormalizedNameToSpecifiedValue.find(sNormalizedName);
		if (iterIndex_MapToSpecifiedValue != m_mapNormalizedNameToSpecifiedValue.end())
		{
			spAppOptionSpecifiedValue = iterIndex_MapToSpecifiedValue->second;
			return S_OK;
		}
	}

	sr = HandleOptionInitialAccess<TValue>(sNormalizedName);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	auto iterIndex_MapToSpecifiedValue = m_mapNormalizedNameToSpecifiedValue.find(sNormalizedName);
	if (iterIndex_MapToSpecifiedValue == m_mapNormalizedNameToSpecifiedValue.end())
	{
		return S_FALSE;
	}

	spAppOptionSpecifiedValue = iterIndex_MapToSpecifiedValue->second;

	return S_OK;
}

} // namespace vlr
