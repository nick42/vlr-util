#pragma once

#include <map>
#include <memory>
#include <shared_mutex>
#include <regex>

#include "config.h"
#include "util.Result.h"
#include "zstring_view.h"

namespace vlr {

#ifdef UNICODE
using tregex = std::wregex;
using tsmatch = std::wsmatch;
#else
using tregex = std::regex;
using tsmatch = std::smatch;
#endif

class RegexCache
{
public:
	using SPRegex = std::shared_ptr<tregex>;

protected:
	std::map<vlr::tstring, SPRegex> m_mapRegexToCompiledRegex;
	mutable std::shared_mutex m_mutexRegexToCompiledRegex;

public:
	SResult GetCompiledRegex(const vlr::tstring& sRegex, SPRegex& spCompiledRegex_Result);
	// Note: Because we currently lookup/store tstring in the map, passing tzstring_view is not not any more efficient
	template <typename TString, typename std::enable_if_t<std::is_convertible_v<TString, vlr::tzstring_view>>* = nullptr>
	inline SResult GetCompiledRegex(const TString& tRegex, SPRegex& spCompiledRegex_Result)
	{
		return GetCompiledRegex(vlr::tzstring_view{ tRegex }.toStdString(), spCompiledRegex_Result);
	}

public:
	static auto& getSharedInstance()
	{
		static auto tSharedInstance = RegexCache{};
		return tSharedInstance;
	}
};

} // namespace vlr
