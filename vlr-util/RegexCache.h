#pragma once

#include <map>
#include <memory>
#include <shared_mutex>
#include <regex>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "util.Result.h"

VLR_NAMESPACE_BEGIN(vlr)

#ifdef UNICODE
using tregex = std::wregex;
#else
using tregex = std::regex;
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

public:
	static auto& getSharedInstance()
	{
		static auto tSharedInstance = RegexCache{};
		return tSharedInstance;
	}
};

VLR_NAMESPACE_END //(vlr)
