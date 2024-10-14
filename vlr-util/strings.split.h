#pragma once

#include <string>
#include <vector>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.StringConversion.h"
#include "util.types.h"
#include "util.Result.h"
#include "StringCompare.h"

namespace vlr {

namespace strings {

template <typename TChar = TCHAR>
struct DelimitersSpec
{
public:
	static constexpr vlr::basic_zstring_view<TChar> GetChars_PathSeparators()
	{
		if constexpr (std::is_same_v<TChar, char>)
		{
			return "/\\";
		}
		else if constexpr (std::is_same_v<TChar, wchar_t>)
		{
			return L"/\\";
		}
		else
		{
			VLR_TYPE_DEPENDENT_STATIC_FAIL(TChar, "Unhanded char type");
		}
	}
	static constexpr vlr::basic_zstring_view<TChar> GetChars_Whitespace()
	{
		if constexpr (std::is_same_v<TChar, char>)
		{
			return " \t\r\n";
		}
		else if constexpr (std::is_same_v<TChar, wchar_t>)
		{
			return L" \t\r\n";
		}
		else
		{
			VLR_TYPE_DEPENDENT_STATIC_FAIL(TChar, "Unhanded char type");
		}
	}

protected:
	static std::vector<TChar> ToStdVector(vlr::basic_zstring_view<TChar> svzChars)
	{
		std::vector<TChar> vecChars;
		for (auto cChar : svzChars)
		{
			vecChars.push_back(cChar);
		}
		return vecChars;
	}

public:
	std::vector<TChar> m_arrDelimiters;

	static const std::vector<TChar>& GetDelimiters_PathSeparators()
	{
		static const auto arrDelimiters = [] {
			return ToStdVector(GetChars_PathSeparators());
		}();
		return arrDelimiters;
	}

	const auto& GetDelimiters() const
	{
		if (m_arrDelimiters.size() > 0)
		{
			return m_arrDelimiters;
		}
		return GetDelimiters_PathSeparators();
	}

public:
	inline static auto ForPaths()
	{
		return DelimitersSpec{ GetDelimiters_PathSeparators()};
	}

public:
	DelimitersSpec() = default;
	DelimitersSpec(const std::vector<TChar>& arrDelimiters)
		: m_arrDelimiters{ arrDelimiters }
	{}
	DelimitersSpec(std::vector<TChar>&& arrDelimiters)
		: m_arrDelimiters{ std::move(arrDelimiters) }
	{}
	DelimitersSpec(TChar cCharacter)
		: m_arrDelimiters{ std::vector<TChar>{cCharacter} }
	{}
};

struct Options_SplitStringAtDelimiter
{
	bool m_bAddEmptyElementsForPrefixDelimiters = false;
	bool m_bAddEmptyElementsForPostfixDelimiters = false;
	bool m_bAddEmptyElementsForConsecutiveDelimiters = false;
};
template <typename TChar>
SResult SplitStringAtDelimiter(const std::basic_string_view<TChar> svString, const DelimitersSpec<TChar>& oDelimitersSpec, std::vector<std::basic_string_view<TChar>>& arrStringElements_Result, const Options_SplitStringAtDelimiter& options = {})
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

template <typename TChar>
SResult SplitStringAtDelimiter_Path(const std::basic_string_view<TChar> svString, std::vector<std::basic_string_view<TChar>>& arrStringElements_Result, const Options_SplitStringAtDelimiter& options = {})
{
	return SplitStringAtDelimiter(svString, DelimitersSpec<TChar>::ForPaths(), arrStringElements_Result, options);
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

template <typename TChar>
std::basic_string_view<TChar> GetTrimmedStringView(std::basic_string_view<TChar> svValue, std::basic_string_view<TChar> svCharsToTrim)
{
	while (svValue.size() > 0)
	{
		auto nIndexMatch_Prefix = svCharsToTrim.find(svValue[0]);
		if (nIndexMatch_Prefix != svCharsToTrim.npos)
		{
			svValue.remove_prefix(1);
			continue;
		}
		auto nIndexMatch_Postfix = svCharsToTrim.find(svValue[svValue.size() - 1]);
		if (nIndexMatch_Postfix != svCharsToTrim.npos)
		{
			svValue.remove_suffix(1);
			continue;
		}
		break;
	}

	return svValue;
}

} // namespace strings

} // namespace vlr
