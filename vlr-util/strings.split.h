#pragma once

#include <string>
#include <vector>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.Unicode.h"
#include "util.types.h"
#include "util.Result.h"
#include "StringCompare.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(strings)

struct DelimitersSpec
{
public:
	std::vector<TCHAR> m_arrDelimiters;

	inline static const auto arrDelimiters_PathSeparators = std::vector<TCHAR>{ _T('/'), _T('\\') };

	const auto& GetDelimiters() const
	{
		if (m_arrDelimiters.size() > 0)
		{
			return m_arrDelimiters;
		}
		return arrDelimiters_PathSeparators;
	}

public:
	inline static auto ForPaths()
	{
		return DelimitersSpec{ arrDelimiters_PathSeparators };
	}

public:
	DelimitersSpec() = default;
	DelimitersSpec(const std::vector<TCHAR>& arrDelimiters)
		: m_arrDelimiters{ arrDelimiters }
	{}
	DelimitersSpec(std::vector<TCHAR>&& arrDelimiters)
		: m_arrDelimiters{ std::move(arrDelimiters) }
	{}
	DelimitersSpec(TCHAR cCharacter)
		: m_arrDelimiters{ std::vector<TCHAR>{cCharacter} }
	{}
};

struct Options_SplitStringAtDelimiter
{
	bool m_bAddEmptyElementsForPrefixDelimiters = false;
	bool m_bAddEmptyElementsForPostfixDelimiters = false;
	bool m_bAddEmptyElementsForConsecutiveDelimiters = false;
};
template< typename TChar >
SResult SplitStringAtDelimiter(const std::basic_string_view<TChar> svString, const DelimitersSpec& oDelimitersSpec, std::vector<std::basic_string_view<TChar>>& arrStringElements_Result, const Options_SplitStringAtDelimiter& options = {})
{
	const auto& arrDelimiters = oDelimitersSpec.GetDelimiters();

	auto fIsDelimiter = [&](TChar cChar)
	{
		return (std::find(arrDelimiters.begin(), arrDelimiters.end(), cChar) != arrDelimiters.end());
	};

	TChar const* pCurrentElementStart = nullptr;
	size_t nCurrentElementLength = 0;
	auto fAddCurrentElementToResults = [&]()
	{
		auto svCurrentElement = std::basic_string_view<TChar>{ pCurrentElementStart, nCurrentElementLength };
		arrStringElements_Result.push_back(svCurrentElement);

		pCurrentElementStart = {};
		nCurrentElementLength = {};
	};
	bool bCurrentCharIsDelimiter = false;

	auto fAddEmptyElementToResults = [&]()
	{
		arrStringElements_Result.push_back(std::basic_string_view<TChar>{});
	};

	for (size_t nIndex = 0; nIndex != svString.size(); ++nIndex)
	{
		// Note: Important to get a reference here, not a copy, since we're going to potentially set a pointer to it
		const auto& cCurrentChar = svString.at(nIndex);
		bCurrentCharIsDelimiter = fIsDelimiter(cCurrentChar);
		if (!bCurrentCharIsDelimiter)
		{
			if (pCurrentElementStart == nullptr)
			{
				pCurrentElementStart = &cCurrentChar;
				nCurrentElementLength = 1;
			}
			else
			{
				nCurrentElementLength++;
			}
			continue;
		}

		// Delimiter has more cases...

		if (nIndex == 0)
		{
			// At the start of the string
			if (!options.m_bAddEmptyElementsForPrefixDelimiters)
			{
				continue;
			}
			fAddEmptyElementToResults();
			continue;
		}
		if (pCurrentElementStart == nullptr)
		{
			// This is the consecutive delimiter case (no current active element)
			if (!options.m_bAddEmptyElementsForConsecutiveDelimiters)
			{
				continue;
			}
			fAddEmptyElementToResults();
			continue;
		}
		// We had an active element, and now reached a delimiter; add string
		fAddCurrentElementToResults();
	}
	if (pCurrentElementStart != nullptr)
	{
		fAddCurrentElementToResults();
	}
	if (bCurrentCharIsDelimiter
		&& options.m_bAddEmptyElementsForPostfixDelimiters)
	{
		fAddEmptyElementToResults();
	}

	return SResult::Success;
}

template< typename TChar >
SResult SplitStringAtDelimiter_Path(const std::basic_string_view<TChar> svString, std::vector<std::basic_string_view<TChar>>& arrStringElements_Result, const Options_SplitStringAtDelimiter& options = {})
{
	return SplitStringAtDelimiter(svString, DelimitersSpec::ForPaths(), arrStringElements_Result, options);
}

template< typename TChar >
vlr::basic_zstring_view<TChar> StringWithoutPossiblePrefix(
	vlr::basic_zstring_view<TChar> svzValue,
	std::basic_string_view<TChar> svPrefix,
	const StringCompare::CComparator& oStringCompare = StringCompare::CS())
{
	if (!oStringCompare.StringHasPrefix(svzValue, svPrefix))
	{
		return svzValue;
	}
	return svzValue.trailing_end(svPrefix.length());
}

template< typename TChar >
std::basic_string_view<TChar> StringWithoutPossiblePostfix(
	vlr::basic_zstring_view<TChar> svzValue,
	std::basic_string_view<TChar> svPostfix,
	const StringCompare::CComparator& oStringCompare = StringCompare::CS())
{
	if (!oStringCompare.StringHasPostfix(svzValue, svPostfix))
	{
		return svzValue;
	}
	return svzValue.substr(0, svzValue.length() - svPostfix.length());
}

VLR_NAMESPACE_END //(strings)

VLR_NAMESPACE_END //(vlr)