#include "pch.h"
#include "RegexCache.h"

#include "logging.LogMessage.h"

namespace vlr {

SResult RegexCache::GetCompiledRegex(const vlr::tstring& sRegex, SPRegex& spCompiledRegex_Result)
{
	{
		auto lockAccess = std::shared_lock{ m_mutexRegexToCompiledRegex };
		auto iterExisting = m_mapRegexToCompiledRegex.find(sRegex);
		if (iterExisting != m_mapRegexToCompiledRegex.end())
		{
			spCompiledRegex_Result = iterExisting->second;
			return SResult::Success;
		}
	}

	// We didn't find it already, so we will compile it and create an entry
	{
		auto lockAccess = std::unique_lock{ m_mutexRegexToCompiledRegex };
		auto& spCompiledRegex = m_mapRegexToCompiledRegex[sRegex];
		// Note: There's a minor race-condition here: we might fail to compile a regex multiple times.

		try
		{
			spCompiledRegex = std::make_shared<tregex>(sRegex);
		}
		catch (const std::exception& ex)
		{
			logging::LogMessagePF(VLR_LOG_CONTEXT_WARNING, _T("Failed to compile regex '%s'; error: %s"),
				sRegex,
				util::Convert::ToStdString(ex.what()));
		}
		// Note: At this point, the map entry is populated (success or failure)

		spCompiledRegex_Result = spCompiledRegex;
		if (!spCompiledRegex_Result)
		{
			return SResult::Success_WithNuance;
		}

		return SResult::Success;
	}
}

} // namespace vlr
