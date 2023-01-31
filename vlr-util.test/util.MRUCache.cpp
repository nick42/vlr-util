#include "pch.h"

#include "vlr-util/util.MRUCache.h"

using namespace vlr::util;

TEST(util_MRUCache, general)
{
	SResult sr;

	auto oCache = vlr::util::CMRUCache<uintptr_t, std::string>{};

	oCache.AddValueToCache(0, "zero");
	oCache.AddValueToCache(42, "theAnswer");

	{
		std::string sCachedValue;
		sr = oCache.GetCachedValue(0, sCachedValue);
		EXPECT_EQ(sr, SResult::Success);
		EXPECT_EQ(sCachedValue, "zero");
	}
	{
		std::string sCachedValue;
		sr = oCache.GetCachedValue(42, sCachedValue);
		EXPECT_EQ(sr, SResult::Success);
		EXPECT_EQ(sCachedValue, "theAnswer");
	}

	oCache.SetCacheSize(1);

	// This should be gone from the cache:
	{
		std::string sCachedValue;
		sr = oCache.GetCachedValue(0, sCachedValue);
		EXPECT_EQ(sr, SResult::Success_NoWorkDone);
	}
	// And this should still be there:
	{
		std::string sCachedValue;
		sr = oCache.GetCachedValue(42, sCachedValue);
		EXPECT_EQ(sr, SResult::Success);
		EXPECT_EQ(sCachedValue, "theAnswer");
	}
}
