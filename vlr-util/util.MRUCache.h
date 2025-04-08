#pragma once

#include "config.h"

#include <list>
#include <unordered_map>

#include "util.Result.h"

namespace vlr {

namespace util {

template< typename TKey, typename TValue >
class CMRUCache
{
protected:
	std::list<TKey> m_oKeyList;
	std::unordered_map<TKey, TValue> m_oKeyValueMap;
	uintptr_t m_nCacheSize = 100;

	SResult TruncateCacheToMaxSize();

public:
	SResult SetCacheSize(uintptr_t nCacheSize);

	SResult GetCachedValue(const TKey& tKey, TValue& tValue);

	SResult AddValueToCache(const TKey& tKey, const TValue& tValue);

};

template< typename TKey, typename TValue >
SResult CMRUCache<TKey, TValue>::TruncateCacheToMaxSize()
{
	while (m_oKeyList.size() > m_nCacheSize)
	{
		const auto& tKey = m_oKeyList.back();
		m_oKeyValueMap.erase(tKey);
		m_oKeyList.pop_back();
	}

	return SResult::Success;
}

template< typename TKey, typename TValue >
SResult CMRUCache<TKey, TValue>::SetCacheSize(uintptr_t nCacheSize)
{
	m_nCacheSize = nCacheSize;

	TruncateCacheToMaxSize();

	return SResult::Success;
}

template< typename TKey, typename TValue >
SResult CMRUCache<TKey, TValue>::GetCachedValue(const TKey& tKey, TValue& tValue)
{
	auto iterIndex = m_oKeyValueMap.find(tKey);
	if (iterIndex == m_oKeyValueMap.end())
	{
		return SResult::Success_NoWorkDone;
	}

	tValue = iterIndex->second;

	return SResult::Success;
}

template< typename TKey, typename TValue >
SResult CMRUCache<TKey, TValue>::AddValueToCache(const TKey& tKey, const TValue& tValue)
{
	m_oKeyList.push_front(tKey);
	m_oKeyValueMap[tKey] = tValue;

	TruncateCacheToMaxSize();

	return SResult::Success;
}

} // namespace util

} // namespace vlr
