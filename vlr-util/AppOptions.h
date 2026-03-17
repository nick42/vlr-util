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

	// The primary collection of all specified option values, keyed by their original/native name.
	// This is the "source of truth" for all specified values (which may not correlate to defined AppOptions).
	// Other collections derive or cache from this collection for optimized access patterns.
	std::unordered_map<vlr::tstring, SPCAppOptionSpecifiedValue> m_mapSpecifiedNameToSpecifiedValue;

	// Cache of specified values keyed by normalized option name, populated on-demand during first access.
	// Stores nullptr for options that were accessed but not specified (to avoid repeated lookups).
	// This optimizes repeated accesses to the same normalized option name. Keys in this map represent
	// all defined AppOptions that have been referenced at least once.
	std::unordered_map<vlr::tstring, SPCAppOptionSpecifiedValue> m_mapNormalizedNameToSpecifiedValue;

	// Option qualifiers, keyed by normalized option name.
	// May be set independently of option values (e.g., before the option is specified or accessed).
	std::unordered_map<vlr::tstring, SPCAppOptionQualifiers> m_mapNormalizedNameToQualifiers;

	// Custom metadata for options, keyed by normalized option name.
	std::unordered_map<vlr::tstring, vlr::tstring> m_mapNormalizedNameToMetadata;

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

	// Returns a const reference to all specified values by their original/native name.
	// WARNING: This is not thread-safe for concurrent modifications. Do not hold this reference
	// across other code or function calls. For thread-safe lookups, use FindSpecifiedValueByName()
	// or PopulateSpecifiedValueForOption() instead.
	inline const auto& GetMap_SpecifiedValues() const
	{
		return m_mapSpecifiedNameToSpecifiedValue;
	}
	// Returns a const reference to the cache of values keyed by normalized name (for accessed options).
	// WARNING: This is not thread-safe for concurrent modifications. Do not hold this reference
	// across other code or function calls. For thread-safe lookups, use PopulateSpecifiedValueForOption()
	// or PopulatePrePreparedSpecifiedValueForOption() instead.
	inline const auto& GetMap_NormalizedNameToSpecifiedValue() const
	{
		return m_mapNormalizedNameToSpecifiedValue;
	}
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

	// Finds all specified values matching the normalized name, including those not yet accessed.
	// Searches the "source of truth" collection and is not optimized for repeated access patterns.
	// Note: May return multiple specified values for the same normalized name.
	SResult FindSpecifiedValuesMatchingNormalizedName(
		const vlr::tstring& sNormalizedName,
		std::vector<SPCAppOptionSpecifiedValue>& vecSpecifiedValues);

	// Populates a specified value for the given normalized option name with the following behaviors:
	// - Finds the value if specified (resolving any potential ambiguity in specification)
	// - Moves the value to the access cache for subsequent quick lookups
	// - Pre-populates the TValue type in the specified value to avoid conversion on later accesses
	// Returns: S_OK if value found, S_FALSE if not specified
	template <typename TValue>
	SResult PopulateSpecifiedValueForOption(
		const vlr::tstring& sNormalizedName,
		SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue);

	// Populates a pre-cached specified value for the given normalized option name.
	// Only returns values which have been previously prepared (via PopulateSpecifiedValueForOption).
	// Returns: S_OK if value exists in cache, S_FALSE if not previously accessed
	SResult PopulatePrePreparedSpecifiedValueForOption(
		const vlr::tstring& sNormalizedName,
		SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue) const;

	SResult ClearPrePopulatedSpecifiedValueForOption(
		const vlr::tstring& sNormalizedName);

	SResult SetAppOptionQualifiers(
		const vlr::tstring& sNormalizedName,
		const SPCAppOptionQualifiers& spAppOptionQualifiers);
	SResult ClearAppOptionQualifiers(
		const vlr::tstring& sNormalizedName);

	SResult SetAppOptionMetadata(
		const vlr::tstring& sNormalizedName,
		const vlr::tstring& sMetadata);
	SResult PopulateAppOptionMetadata(
		const vlr::tstring& sNormalizedName,
		vlr::tstring& sMetadata) const;
	SResult ClearAppOptionMetadata(
		const vlr::tstring& sNormalizedName);

	// Clears the specified value. The value stored in the parameter is ignored; identification
	// is based on the value's internal name.
	SResult ClearSpecifiedValue(
		const SPCAppOptionSpecifiedValue& spAppOptionSpecifiedValue);

	// Clears all specified values and resets the internal access cache.
	SResult ClearAllSpecifiedValues();

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
