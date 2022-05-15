#include "pch.h"

#include "vlr-util/RegexCache.h"

#include <thread>

using vlr::SResult;
using vlr::RegexCache;

TEST(RegexCache, BasicSuccess)
{
	SResult sr;

	static constexpr auto svzRegex_Good = _T(".*");

	RegexCache::SPRegex spCompiledRegex;
	sr = RegexCache::getSharedInstance().GetCompiledRegex(svzRegex_Good, spCompiledRegex);
	ASSERT_EQ(sr, SResult::GeneralSuccess);
	ASSERT_NE(spCompiledRegex, nullptr);

	RegexCache::SPRegex spCompiledRegex_Copy;
	sr = RegexCache::getSharedInstance().GetCompiledRegex(svzRegex_Good, spCompiledRegex_Copy);
	ASSERT_EQ(sr, SResult::GeneralSuccess);
	ASSERT_NE(spCompiledRegex_Copy, nullptr);

	ASSERT_EQ(spCompiledRegex.get(), spCompiledRegex_Copy.get());
}

TEST(RegexCache, BasicFailure)
{
	SResult sr;

	static constexpr auto svzRegex_Good = _T("(.*");

	RegexCache::SPRegex spCompiledRegex;
	sr = RegexCache::getSharedInstance().GetCompiledRegex(svzRegex_Good, spCompiledRegex);
	ASSERT_EQ(sr, SResult::SuccessWithNuance);
	ASSERT_EQ(spCompiledRegex, nullptr);
}

TEST(RegexCache, BasicSuccessMultiThreaded)
{
	SResult sr;

	static constexpr auto svzRegex_Good = _T(".*");

	auto fGetCompiledRegex = [&]
	{
		RegexCache::SPRegex spCompiledRegex;
		sr = RegexCache::getSharedInstance().GetCompiledRegex(svzRegex_Good, spCompiledRegex);
		ASSERT_EQ(sr, SResult::GeneralSuccess);
		ASSERT_NE(spCompiledRegex, nullptr);
	};

	std::vector<std::thread> vecThreads;
	for (auto n = 0; n < 100; ++n)
	{
		vecThreads.push_back(std::thread{ fGetCompiledRegex });
	}
	for (auto& oThread : vecThreads)
	{
		oThread.join();
	}
}
