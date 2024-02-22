#include "pch.h"
#include "AppOptionValue.h"

#include "strings.split.h"

namespace vlr {

SResult CAppOptionValue::CheckForOptionMatch(
	const vlr::tstring& sNormalizedOptionName,
	const std::vector<vlr::tstring_view>& vecOptionNameElements)
{
	SResult sr;

	auto oNameTreeDelimiters = strings::DelimitersSpec<TCHAR>{ std::vector<TCHAR>{ _T(':'), _T('.') } };

	std::vector<vlr::tstring_view> vecOurOptionNameElements;
	sr = strings::SplitStringAtDelimiter<TCHAR>(m_sNativeOptionName, oNameTreeDelimiters, vecOurOptionNameElements);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	auto oStringCompare_OptionNameTree = StringCompare::CI();

	if (vecOurOptionNameElements.size() != vecOptionNameElements.size())
	{
		return S_FALSE;
	}
	for (size_t i = 0; i < vecOurOptionNameElements.size(); ++i)
	{
		if (!oStringCompare_OptionNameTree.AreEqual(vecOurOptionNameElements[i], vecOptionNameElements[i]))
		{
			return S_FALSE;
		}
	}

	// Match to option

	m_sNormalizedOptionName = sNormalizedOptionName;

	return S_OK;
}

} // namespace vlr
