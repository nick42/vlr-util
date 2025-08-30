#pragma once

#include "vlr-util/config.h"
#include "vlr-util/util.Result.h"
#include "vlr-util/AppOptions.h"
#include "vlr-util/RegexCache.h"

namespace vlr {

namespace AppOptionSource_File {

class CSourceContext
{
public:
	vlr::tstring m_sFilePath;

	inline auto& withFilePath(vlr::tzstring_view svzFilePath)
	{
		m_sFilePath = svzFilePath;
		return *this;
	}
};

} // namespace AppOptionSource_File

class CAppOptionSource_File_Basic
{
	friend class AppOptionSource_File_Basic_Test;

protected:
	CAppOptions* m_pAppOptions_Override = nullptr;

	inline auto& GetAppOptions()
	{
		if (m_pAppOptions_Override)
		{
			return *m_pAppOptions_Override;
		}
		else
		{
			return CAppOptions::GetSharedInstance();
		}
	}

public:
	constexpr auto& withAppOptionsOverride(CAppOptions* pAppOptions) noexcept
	{
		m_pAppOptions_Override = pAppOptions;
		return *this;
	}

protected:
	static constexpr vlr::tzstring_view m_svzRegex_BlankLine = _T("^\\s*$");
	RegexCache::SPRegex m_spRegex_BlankLine;
	static constexpr vlr::tzstring_view m_svzRegex_CommentLine = _T("^\\s*#.*$");
	RegexCache::SPRegex m_spRegex_CommentLine;
	static constexpr vlr::tzstring_view m_svzRegex_NameValueLine = _T("^\\s*(.+?)\\s*=\\s*(.*?)$");
	RegexCache::SPRegex m_spRegex_NameValueLine;
	static constexpr vlr::tzstring_view m_svzRegex_QuotedValue = _T("^\"((?:[^\"\\\\]|\\\\.)*)\".*$");
	RegexCache::SPRegex m_spRegex_QuotedValue;

protected:
	// Nick's note: The current algorithm here is pretty inefficient; lots of string copies in memory. Ideally
	// this would probably use memory mapped files and direct reads where possible. This is not currently 
	// very possible, because it would require using something like boost, which I cannot add as a dependency.

	SResult ReadAllValuesFromFile_WithinTryCatch(
		vlr::tzstring_view svzFilePath);
	SResult EnsureRegexMembersInitialized();
	SResult ParseFileLine(
		const AppOptionSource_File::CSourceContext& oSourceContext,
		const std::string& saLine);
	SResult ParseFileLine_NameValueLine(
		const AppOptionSource_File::CSourceContext& oSourceContext,
		const tstring& sLine,
		const tsmatch& oRegexMatch_NameValueLine);
	SResult AddSpecifiedOptionValueFromFile(
		const AppOptionSource_File::CSourceContext& oSourceContext,
		vlr::tstring_view svName,
		vlr::tstring_view svValue);

public:
	SResult ReadAllValuesFromFile(
		vlr::tzstring_view svzFilePath);

};

} // namespace vlr
