#include "pch.h"
#include "AppOptionSource_File_Basic.h"

#include <fstream>
#include <iostream>

#include "vlr-util/strings.split.h"

namespace vlr {

SResult CAppOptionSource_File_Basic::ReadAllValuesFromFile_WithinTryCatch(
	vlr::tzstring_view svzFilePath)
{
	auto oFileInputStream = std::ifstream{ svzFilePath };
	if (!oFileInputStream.is_open())
	{
		return SResult::Failure;
	}

	SResult sr;

	sr = EnsureRegexMembersInitialized();
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	auto oSourceContext = AppOptionSource_File::CSourceContext{}.withFilePath(svzFilePath);

	std::string saLine;
	while (std::getline(oFileInputStream, saLine))
	{
		sr = ParseFileLine(oSourceContext, saLine);
		VLR_ASSERT_SR_SUCCEEDED_OR_CONTINUE(sr);
	}

	return SResult::Success;
}

SResult CAppOptionSource_File_Basic::EnsureRegexMembersInitialized()
{
	SResult sr;

	if (!m_spRegex_BlankLine)
	{
		sr = RegexCache::getSharedInstance().GetCompiledRegex(m_svzRegex_BlankLine, m_spRegex_BlankLine);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}

	if (!m_spRegex_CommentLine)
	{
		sr = RegexCache::getSharedInstance().GetCompiledRegex(m_svzRegex_CommentLine, m_spRegex_CommentLine);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}

	if (!m_spRegex_NameValueLine)
	{
		sr = RegexCache::getSharedInstance().GetCompiledRegex(m_svzRegex_NameValueLine, m_spRegex_NameValueLine);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}

	if (!m_spRegex_QuotedValue)
	{
		sr = RegexCache::getSharedInstance().GetCompiledRegex(m_svzRegex_QuotedValue, m_spRegex_QuotedValue);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	}

	return SResult::Success;
}

SResult CAppOptionSource_File_Basic::ParseFileLine(
	const AppOptionSource_File::CSourceContext& oSourceContext,
	const std::string& saLine)
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(m_spRegex_BlankLine);
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(m_spRegex_CommentLine);
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(m_spRegex_NameValueLine);

	auto sLine = util::Convert::ToStdString(saLine);

	bool bMatch_BlankLine = std::regex_match(sLine, *m_spRegex_BlankLine);
	if (bMatch_BlankLine)
	{
		return SResult::Success_WithNuance;
	}
	bool bMatch_CommentLine = std::regex_match(sLine, *m_spRegex_CommentLine);
	if (bMatch_CommentLine)
	{
		return SResult::Success_WithNuance;
	}
	tsmatch oRegexMatch_NameValueLine;
	bool bMatch_NameValueLine = std::regex_match(sLine, oRegexMatch_NameValueLine, *m_spRegex_NameValueLine);
	if (bMatch_NameValueLine)
	{
		return ParseFileLine_NameValueLine(oSourceContext, sLine, oRegexMatch_NameValueLine);
	}

	// Unknown line... ignoring
	fmt::print(_T("Ignoring presumed invalid line:\n\t{}\n"), sLine);

	return SResult::Success_NoWorkDone;
}

SResult CAppOptionSource_File_Basic::ParseFileLine_NameValueLine(
	const AppOptionSource_File::CSourceContext& oSourceContext,
	const tstring& sLine,
	const tsmatch& oRegexMatch_NameValueLine)
{
	fmt::print(_T("Parsed line as name/value:\n\t{}\n"), sLine);

	// Note: There are a couple valid cases to consider here:
	// - blank value
	// - value in quotes
	// - extra comments on the end of the line

	const auto& oMatchName = oRegexMatch_NameValueLine[1];
	auto svMatchName = vlr::tstring_view{ &(*oMatchName.first), static_cast<size_t>(oMatchName.second - oMatchName.first) };
	svMatchName = strings::GetTrimmedStringView(svMatchName, strings::DelimitersSpec<TCHAR>::GetChars_Whitespace());

	if (svMatchName.size() == 0)
	{
		return S_FALSE;
	}

	const auto& oMatchValue = oRegexMatch_NameValueLine[2];
	// Note: This is better/added in C++20; this is the hacky way:
	auto svMatchValue = vlr::tstring_view{ &(*oMatchValue.first), static_cast<size_t>(oMatchValue.second - oMatchValue.first) };
	svMatchValue = strings::GetTrimmedStringView(svMatchValue, strings::DelimitersSpec<TCHAR>::GetChars_Whitespace());

	if (svMatchValue.size() == 0)
	{
		return AddSpecifiedOptionValueFromFile(oSourceContext, svMatchName, svMatchValue);
	}

	// Note: This is inefficient, but std::regex_match doesn't play nice with string_view :/
	vlr::tstring sMatchValue{ svMatchValue.begin(), svMatchValue.end() };

	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(m_spRegex_QuotedValue);

	tsmatch oRegexMatch_QuotedValue;
	bool bMatch_QuotedValue = std::regex_match(sMatchValue, oRegexMatch_QuotedValue, *m_spRegex_QuotedValue);
	if (bMatch_QuotedValue)
	{
		// Note: We ignore anything beyond the quoted identifier
		const auto& oMatchQuotedValue = oRegexMatch_QuotedValue[1];
		auto svQuotedValue = vlr::tstring_view{ &(*oMatchQuotedValue.first), static_cast<size_t>(oMatchQuotedValue.second - oMatchQuotedValue.first) };
		return AddSpecifiedOptionValueFromFile(oSourceContext, svMatchName, svQuotedValue);
	}

	auto nIndex_CommentStart = vlr::tstring_view::npos;
	// Note: Code structure allows for possible multiple chars in the future...
	do
	{
		nIndex_CommentStart = svMatchValue.find(_T('#'));
		if (nIndex_CommentStart != vlr::tstring_view::npos)
		{
			svMatchValue = svMatchValue.substr(0, nIndex_CommentStart);
		}
	} while (false);
	// We may now have extra whitespace...
	svMatchValue = strings::GetTrimmedStringView(svMatchValue, strings::DelimitersSpec<TCHAR>::GetChars_Whitespace());

	return AddSpecifiedOptionValueFromFile(oSourceContext, svMatchName, svMatchValue);
}

SResult CAppOptionSource_File_Basic::AddSpecifiedOptionValueFromFile(
	const AppOptionSource_File::CSourceContext& oSourceContext,
	vlr::tstring_view svName,
	vlr::tstring_view svValue)
{
	fmt::print(_T("Adding option name/value from file:\n\t{}: {}\n"), svName, svValue);

	auto spSpecifiedValue = std::make_shared<CAppOptionSpecifiedValue>(
		CAppOptionSourceInfo{ AppOptionSource::BinaryRelatedConfigFile, oSourceContext.m_sFilePath },
		svName,
		svValue);

	return GetAppOptions().AddSpecifiedValue(spSpecifiedValue);
}

SResult CAppOptionSource_File_Basic::ReadAllValuesFromFile(
	vlr::tzstring_view svzFilePath)
{
	try
	{
		return ReadAllValuesFromFile_WithinTryCatch(svzFilePath);
	}
	catch (const std::exception& /*ex*/)
	{
		// TODO: Translate to specific code
		return SResult::Failure;
	}
}

} // namespace vlr
