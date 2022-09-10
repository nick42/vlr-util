#pragma once

#include <cctype>
#include <cwchar>
#include <string>
#include <string_view>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.choice.h"
#include "util.types.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(StringCompare)

VLR_NAMESPACE_BEGIN(detail)

template<typename TString>
constexpr bool isCompatTypeForAString()
{
	if constexpr (std::is_base_of_v<std::string_view, TString> || std::is_convertible_v<TString, std::string>)
	{
		return true;
	}

	return false;
}

template<typename TString>
constexpr bool isCompatTypeForWString()
{
	if constexpr (std::is_base_of_v<std::wstring_view, TString> || std::is_convertible_v<TString, std::wstring>)
	{
		return true;
	}

	return false;
}

template<typename TString, typename = std::enable_if_t<std::is_null_pointer_v<TString>>>
constexpr decltype(auto) choice_asAStringViewCompatType(const TString& tValue, vlr::util::choice<0>&&)
{
	return std::string_view{};
}

template<typename TString, typename = std::enable_if_t<std::is_base_of_v<std::string_view, TString>>>
constexpr decltype(auto) choice_asAStringViewCompatType(const TString& tValue, vlr::util::choice<2>&&)
{
	return static_cast<std::string_view>(tValue);
}

template<typename TString, typename = std::enable_if_t<std::is_base_of_v<std::string, TString>>>
constexpr decltype(auto) choice_asAStringViewCompatType(const TString& tValue, vlr::util::choice<3>&&)
{
	return tValue;
}

template<typename TString, typename = std::enable_if_t<std::is_convertible_v<TString, std::string>>>
constexpr decltype(auto) choice_asAStringViewCompatType(const TString& tValue, vlr::util::choice<4>&&)
{
	return static_cast<std::string>(tValue);
}

template<typename TString, typename = std::enable_if_t<std::is_pointer_v<TString> && std::is_convertible_v<TString, std::string>>>
constexpr decltype(auto) choice_asAStringViewCompatType(const TString& tValue, vlr::util::choice<1>&&)
{
	using TConversionResult = decltype(detail::choice_asAStringViewCompatType(std::declval<TString>(), std::declval<vlr::util::choice<2>>()));
	if (tValue == nullptr)
	{
		return TConversionResult{};
	}
	return detail::choice_asAStringViewCompatType(tValue, vlr::util::choice<2>{});
}

template<typename TString, typename = std::enable_if_t<std::is_null_pointer_v<TString>>>
constexpr decltype(auto) choice_asWStringViewCompatType(const TString& tValue, vlr::util::choice<0>&&)
{
	return std::wstring_view{};
}

template<typename TString, typename = std::enable_if_t<std::is_base_of_v<std::wstring_view, TString>>>
constexpr decltype(auto) choice_asWStringViewCompatType(const TString& tValue, vlr::util::choice<2>&&)
{
	return static_cast<std::wstring_view>(tValue);
}

template<typename TString, typename = std::enable_if_t<std::is_base_of_v<std::wstring, TString>>>
constexpr decltype(auto) choice_asWStringViewCompatType(const TString& tValue, vlr::util::choice<3>&&)
{
	return tValue;
}

template<typename TString, typename = std::enable_if_t<std::is_convertible_v<TString, std::wstring>>>
constexpr decltype(auto) choice_asWStringViewCompatType(const TString& tValue, vlr::util::choice<4>&&)
{
	return static_cast<std::wstring>(tValue);
}

template<typename TString, typename = std::enable_if_t<std::is_pointer_v<TString> && std::is_convertible_v<TString, std::wstring>>>
constexpr decltype(auto) choice_asWStringViewCompatType(const TString& tValue, vlr::util::choice<1>&&)
{
	using TConversionResult = decltype(detail::choice_asWStringViewCompatType(std::declval<TString>(), std::declval<vlr::util::choice<2>>()));
	if (tValue == nullptr)
	{
		return TConversionResult{};
	}
	return detail::choice_asWStringViewCompatType(tValue, vlr::util::choice<2>{});
}

VLR_NAMESPACE_END //(detail)

template<typename TValue>
constexpr decltype(auto) asAStringViewCompatType(const TValue& tValue)
{
	return detail::choice_asAStringViewCompatType(tValue, vlr::util::choice<0>{});
}

template<typename TValue>
constexpr decltype(auto) asWStringViewCompatType(const TValue& tValue)
{
	return detail::choice_asWStringViewCompatType(tValue, vlr::util::choice<0>{});
}

// Note: The "fallback" case is the value itself...
template<typename TValue>
using string_view_compat_t = 
	std::conditional_t<detail::isCompatTypeForAString<TValue>(), std::string_view,
	std::conditional_t<detail::isCompatTypeForWString<TValue>(), std::wstring_view,
	TValue>>;

template<typename TValue>
constexpr decltype(auto) asStringViewCompatType(const TValue& tValue)
{
	if constexpr (detail::isCompatTypeForAString<TValue>())
	{
		return asAStringViewCompatType(tValue);
	}
	else if constexpr (detail::isCompatTypeForWString<TValue>())
	{
		return asWStringViewCompatType(tValue);
	}
	else
	{
		return tValue;
	}
}

VLR_NAMESPACE_BEGIN(CaseSensitivity)

enum ECaseSensitivity
{
	Unknown,
	CaseSensitive,
	CaseInsensitive,
};

VLR_NAMESPACE_END //(CaseSensitivity)

struct CompareSettings
{
	CaseSensitivity::ECaseSensitivity m_eCaseSensitivity = CaseSensitivity::CaseSensitive;

	decltype(auto) withCaseSensitivity(CaseSensitivity::ECaseSensitivity eCaseSensitivity)
	{
		m_eCaseSensitivity = eCaseSensitivity;
		return *this;
	}

	static constexpr auto ForCaseSensitive()
	{
		CompareSettings oSettings;
		oSettings.m_eCaseSensitivity = CaseSensitivity::CaseSensitive;
		return oSettings;
	}
	static constexpr auto ForCaseInsensitive()
	{
		CompareSettings oSettings;
		oSettings.m_eCaseSensitivity = CaseSensitivity::CaseInsensitive;
		return oSettings;
	}
};

VLR_NAMESPACE_BEGIN(detail)

// Note: This is an alternative method, per cpp docs; not currently used, as may not be optimal.
// (ie: C-runtime methods for direct comparison without conversion might be faster)
// To be tested in the future.

template<typename TChar>
struct char_traits_ci : public std::char_traits<TChar> {
	static char to_upper(TChar ch) {
		return std::toupper(ch);
	}
	static bool eq(TChar c1, TChar c2) {
		return to_upper(c1) == to_upper(c2);
	}
	static bool lt(TChar c1, TChar c2) {
		return to_upper(c1) < to_upper(c2);
	}
	static int compare(const TChar* s1, const TChar* s2, std::size_t n) {
		while (n-- != 0) {
			if (to_upper(*s1) < to_upper(*s2)) return -1;
			if (to_upper(*s1) > to_upper(*s2)) return 1;
			++s1; ++s2;
		}
		return 0;
	}
	static const TChar* find(const TChar* s, std::size_t n, TChar a) {
		auto const ua(to_upper(a));
		while (n-- != 0)
		{
			if (to_upper(*s) == ua)
				return s;
			s++;
		}
		return nullptr;
	}
};

template<class DstTraits, class CharT, class SrcTraits>
constexpr auto cast_string_view_with_traits(const std::basic_string_view<CharT, SrcTraits> src) noexcept
{
	return std::basic_string_view<CharT, DstTraits>{ src.data(), src.size() };
}

template<typename TString>
std::wstring GetAsWString(const TString& tValue)
{
	if constexpr (isCompatTypeForAString<TString>())
	{
		const auto& saValue = asAStringViewCompatType(tValue);
		const auto svaValue = static_cast<std::string_view>(saValue);
		if (svaValue.size() == 0)
		{
			return {};
		}

		std::wstring swResult;
		swResult.resize(saValue.length());
		size_t nResultLength = 0;

		size_t nSourceIndex = 0;
		std::mbstate_t state = std::mbstate_t();
		for (size_t nResultIndex = 0; nResultIndex < swResult.size(); ++nResultIndex)
		{
			auto nRemainingSourceSize = svaValue.size() - nSourceIndex;
			if (nRemainingSourceSize <= 0)
			{
				break;
			}

			auto nConversionResult = std::mbrtowc(swResult.data() + nResultIndex, svaValue.data() + nSourceIndex, nRemainingSourceSize, &state);
			if (nConversionResult == 0)
			{
				break;
			}
			if (nConversionResult == static_cast<size_t>(-1))
			{
				// Encoding error
				break;
			}
			if (nConversionResult == static_cast<size_t>(-2))
			{
				// Partial read, then ran out of characters
				break;
			}
			nSourceIndex += nConversionResult;
			nResultLength++;
		}
		swResult.resize(nResultLength);

		return swResult;
	}
	else
	{
		static_assert(VLR_DEPENDENT_FALSE, "Unhandled conversion case");
	}

	return {};
}

template<typename TStringView>
decltype(auto) StringViewCompareCS_ToShorterLength(TStringView svlhs, TStringView svrhs)
{
	auto nSmallerLength = std::min(svlhs.size(), svrhs.size());
	if constexpr (std::is_same_v<TStringView, std::string_view>)
	{
		return strncmp(svlhs.data(), svrhs.data(), nSmallerLength);
	}
	else if constexpr (std::is_same_v<TStringView, std::wstring_view>)
	{
		return wcsncmp(svlhs.data(), svrhs.data(), nSmallerLength);
	}
	else
	{
		static_assert(VLR_DEPENDENT_FALSE, "Unhandled string_view type");
	}
}

template<typename TStringView>
decltype(auto) StringViewCompareCS(TStringView svlhs, TStringView svrhs)
{
	auto nSmallerLength = std::min(svlhs.size(), svrhs.size());
	auto nResultToSmallerLength = StringViewCompareCS_ToShorterLength(svlhs, svrhs);
	if (nResultToSmallerLength != 0)
	{
		return nResultToSmallerLength;
	}
	// Shorter string is lessor in normal algorithms...
	if (svrhs.size() > nSmallerLength)
	{
		// lhs was shorter
		return -1;
	}
	if (svlhs.size() > nSmallerLength)
	{
		// rhs was shorter
		return 1;
	}
	return 0;
}

// Note: AreEqual[...] is a more efficient version of Compare[...], because we can short circuit on
// inequality of string length. Compare semantics is return inequality in shared prefix.
// See: https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strnicmp-wcsnicmp-mbsnicmp-strnicmp-l-wcsnicmp-l-mbsnicmp-l?view=msvc-170

template<typename TStringView>
decltype(auto) StringViewCompareCI_ToShorterLength_Compat(TStringView svlhs, TStringView svrhs)
{
	auto nSmallerLength = std::min(svlhs.size(), svrhs.size());
	for (decltype(nSmallerLength) i = 0; i < nSmallerLength; ++i)
	{
		auto clhsUC = std::toupper(svlhs[i]);
		auto crhsUC = std::toupper(svrhs[i]);
		if (clhsUC < crhsUC)
		{
			return -1;
		}
		if (clhsUC > crhsUC)
		{
			return 1;
		}
	}

	return 0;
}

template<typename TStringView>
decltype(auto) StringViewCompareCI_ToShorterLength(TStringView svlhs, TStringView svrhs)
{
	if constexpr (ModuleContext::Compilation::IsBuildPlatform_Win32())
	{
		auto nSmallerLength = std::min(svlhs.size(), svrhs.size());
		if constexpr (std::is_same_v<TStringView, std::string_view>)
		{
			return _strnicmp(svlhs.data(), svrhs.data(), nSmallerLength);
		}
		else if constexpr (std::is_same_v<TStringView, std::wstring_view>)
		{
			return _wcsnicmp(svlhs.data(), svrhs.data(), nSmallerLength);
		}
		else
		{
			static_assert(VLR_DEPENDENT_FALSE, "Unhandled string_view type");
		}
	}
	else
	{
		return StringViewCompareCI_ToShorterLength_Compat(svlhs, svrhs);
	}
}

template<typename TStringView>
decltype(auto) StringViewCompareCI(TStringView svlhs, TStringView svrhs)
{
	auto nSmallerLength = std::min(svlhs.size(), svrhs.size());
	auto nResultToSmallerLength = StringViewCompareCI_ToShorterLength(svlhs, svrhs);
	if (nResultToSmallerLength != 0)
	{
		return nResultToSmallerLength;
	}
	// Shorter string is lessor in normal algorithms...
	if (svrhs.size() > nSmallerLength)
	{
		// lhs was shorter
		return -1;
	}
	if (svlhs.size() > nSmallerLength)
	{
		// rhs was shorter
		return 1;
	}
	return 0;
}

template<typename TStringView>
constexpr decltype(auto) AreEqualStringViews(const CompareSettings& oCompareSettings, const TStringView& svlhs, const TStringView& svrhs)
{
	// Short-circuit for same buffer
	if (svlhs.data() == svrhs.data())
	{
		return true;
	}
	// Note: We allow strings which are "equivalent blank" to compare to true; this allows a blank string 
	// compare will a NULL string to return true
	if ((svlhs.size() == 0) && (svrhs.size() == 0))
	{
		return true;
	}

	switch (oCompareSettings.m_eCaseSensitivity)
	{
	default:
	case CaseSensitivity::CaseSensitive:
		// Assuming std compare is efficient
		return (svlhs == svrhs);

	case CaseSensitivity::CaseInsensitive:
		return true
			&& (svlhs.size() == svrhs.size())
			// Note: C runtime function only works with non-NULL pointers, so short-circuit those cases
			// Will return false for either of (but not both) pointers NULL
			&& (svlhs.data() != nullptr)
			&& (svrhs.data() != nullptr)
			// TODO: Make sure this works on multiple platforms...
			&& (StringViewCompareCI(svlhs, svrhs) == 0)
			;
	}
}

template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(isCompatTypeForAString<Tlhs>() && isCompatTypeForAString<Trhs>())>>
constexpr decltype(auto) AreEqualAsAStringCompat(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
{
	const auto& slhs = asAStringViewCompatType(tlhs);
	const auto svlhs = static_cast<const string_view_compat_t<Tlhs>>(slhs);
	const auto& srhs = asAStringViewCompatType(trhs);
	const auto svrhs = static_cast<const string_view_compat_t<Trhs>>(srhs);

	return AreEqualStringViews(oCompareSettings, svlhs, svrhs);
}

template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(isCompatTypeForWString<Tlhs>() && isCompatTypeForWString<Trhs>())>>
constexpr decltype(auto) AreEqualAsWStringCompat(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
{
	const auto& slhs = asWStringViewCompatType(tlhs);
	const auto svlhs = static_cast<const string_view_compat_t<Tlhs>>(slhs);
	const auto& srhs = asWStringViewCompatType(trhs);
	const auto svrhs = static_cast<const string_view_compat_t<Trhs>>(srhs);

	return AreEqualStringViews(oCompareSettings, svlhs, svrhs);
}

template<typename Tlhs, typename Trhs>
constexpr decltype(auto) AreEqualDifferentCharSize(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
{
	// Going to widen for comparison
	std::wstring swConvertedWString_lhs;
	std::wstring swConvertedWString_rhs;
	std::wstring_view svlhs;
	std::wstring_view svrhs;

	if constexpr (!isCompatTypeForWString<Tlhs>())
	{
		swConvertedWString_lhs = GetAsWString(tlhs);
		svlhs = swConvertedWString_lhs;
	}
	else if constexpr (std::is_base_of_v<std::wstring_view, decltype(asWStringViewCompatType(tlhs))>)
	{
		svlhs = asWStringViewCompatType(tlhs);
	}
	else
	{
		swConvertedWString_lhs = asWStringViewCompatType(tlhs);
		svlhs = swConvertedWString_lhs;
	}

	if constexpr (!isCompatTypeForWString<Trhs>())
	{
		swConvertedWString_rhs = GetAsWString(trhs);
		svrhs = swConvertedWString_rhs;
	}
	else if constexpr (std::is_base_of_v<std::wstring_view, decltype(asWStringViewCompatType(trhs))>)
	{
		svrhs = asWStringViewCompatType(trhs);
	}
	else
	{
		swConvertedWString_rhs = asWStringViewCompatType(trhs);
		svrhs = swConvertedWString_rhs;
	}

	return AreEqualStringViews(oCompareSettings, svlhs, svrhs);
}

template<typename TStringView>
constexpr decltype(auto) CompareStringViews(const CompareSettings& oCompareSettings, const TStringView& svlhs, const TStringView& svrhs)
{
	// Short-circuit for same buffer
	if (svlhs.data() == svrhs.data())
	{
		return 0;
	}
	// Note: We allow strings which are "equivalent blank" to compare to true; this allows a blank string 
	// compare will a NULL string to return true
	if ((svlhs.size() == 0) && (svrhs.size() == 0))
	{
		return 0;
	}

	switch (oCompareSettings.m_eCaseSensitivity)
	{
	default:
	case CaseSensitivity::CaseSensitive:
		// Assuming std compare is efficient
		return StringViewCompareCS(svlhs, svrhs);

	case CaseSensitivity::CaseInsensitive:
		if (svlhs.data() == nullptr)
		{
			// lhs was "shorter"
			return -1;
		}
		if (svrhs.data() == nullptr)
		{
			// rhs was "shorter"
			return 1;
		}
		return StringViewCompareCI(svlhs, svrhs);
	}
}

template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(isCompatTypeForAString<Tlhs>() && isCompatTypeForAString<Trhs>())>>
constexpr decltype(auto) CompareAsAStringCompat(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
{
	const auto& slhs = asAStringViewCompatType(tlhs);
	const auto svlhs = static_cast<const string_view_compat_t<Tlhs>>(slhs);
	const auto& srhs = asAStringViewCompatType(trhs);
	const auto svrhs = static_cast<const string_view_compat_t<Trhs>>(srhs);

	return CompareStringViews(oCompareSettings, svlhs, svrhs);
}

template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(isCompatTypeForWString<Tlhs>() && isCompatTypeForWString<Trhs>())>>
constexpr decltype(auto) CompareAsWStringCompat(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
{
	const auto& slhs = asWStringViewCompatType(tlhs);
	const auto svlhs = static_cast<const string_view_compat_t<Tlhs>>(slhs);
	const auto& srhs = asWStringViewCompatType(trhs);
	const auto svrhs = static_cast<const string_view_compat_t<Trhs>>(srhs);

	return CompareStringViews(oCompareSettings, svlhs, svrhs);
}

template<typename Tlhs, typename Trhs>
constexpr decltype(auto) CompareDifferentCharSize(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
{
	// Going to widen for comparison
	std::wstring swConvertedWString_lhs;
	std::wstring swConvertedWString_rhs;
	std::wstring_view svlhs;
	std::wstring_view svrhs;

	if constexpr (!isCompatTypeForWString<Tlhs>())
	{
		swConvertedWString_lhs = GetAsWString(tlhs);
		svlhs = swConvertedWString_lhs;
	}
	else if constexpr (std::is_base_of_v<std::wstring_view, decltype(asWStringViewCompatType(tlhs))>)
	{
		svlhs = asWStringViewCompatType(tlhs);
	}
	else
	{
		swConvertedWString_lhs = asWStringViewCompatType(tlhs);
		svlhs = swConvertedWString_lhs;
	}

	if constexpr (!isCompatTypeForWString<Trhs>())
	{
		swConvertedWString_rhs = GetAsWString(trhs);
		svrhs = swConvertedWString_rhs;
	}
	else if constexpr (std::is_base_of_v<std::wstring_view, decltype(asWStringViewCompatType(trhs))>)
	{
		svrhs = asWStringViewCompatType(trhs);
	}
	else
	{
		swConvertedWString_rhs = asWStringViewCompatType(trhs);
		svrhs = swConvertedWString_rhs;
	}

	return CompareStringViews(oCompareSettings, svlhs, svrhs);
}

// Note: It would be more efficient to do inline per-character compare, rather than conversion; 
// do this in the future as desired.

//template<typename Tlhs, typename Trhs>
//constexpr decltype(auto) AreEqualDifferentCharSize(const CompareSettings& oCompareSettings, const Tlhs& tlhs, const Trhs& trhs)
//{
//	size_t nCurrentIndex_lhs = 0;
//	size_t nCurrentIndex_rhs = 0;
//
//	auto fWCharGenerator_lhs = [&]()
//	{
//		if constexpr (std::is_base_of_v<std::wstring_view, decltype(asWStringViewCompatType(tlhs))>)
//		{
//			auto svw
//		}
//	};
//}

VLR_NAMESPACE_END //(detail)

struct CComparator
{
	CompareSettings m_oCompareSettings;

protected:
	template<typename TString, typename = std::enable_if_t<detail::isCompatTypeForAString<TString>()>>
	static constexpr auto choice_IsBlank(const TString& tValue, vlr::util::choice<0>&&)
	{
		return static_cast<std::string_view>(asAStringViewCompatType(tValue)).empty();
	}
	template<typename TString, typename = std::enable_if_t<detail::isCompatTypeForWString<TString>()>>
	static constexpr auto choice_IsBlank(const TString& tValue, vlr::util::choice<1>&&)
	{
		return static_cast<std::wstring_view>(asWStringViewCompatType(tValue)).empty();
	}

	template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(detail::isCompatTypeForAString<Tlhs>() && detail::isCompatTypeForAString<Trhs>())>>
	constexpr decltype(auto) choice_AreEqual(const Tlhs& tlhs, const Trhs& trhs, vlr::util::choice<0>&&) const
	{
		return detail::AreEqualAsAStringCompat(m_oCompareSettings, tlhs, trhs);
	}
	template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(detail::isCompatTypeForWString<Tlhs>() && detail::isCompatTypeForWString<Trhs>())>>
	constexpr decltype(auto) choice_AreEqual(const Tlhs& tlhs, const Trhs& trhs, vlr::util::choice<1>&&) const
	{
		return detail::AreEqualAsWStringCompat(m_oCompareSettings, tlhs, trhs);
	}
	// No direct compare possible as this point; some conversion will be necessary
	template<typename Tlhs, typename Trhs>
	constexpr decltype(auto) choice_AreEqual(const Tlhs& tlhs, const Trhs& trhs, vlr::util::choice<2>&&) const
	{
		return detail::AreEqualDifferentCharSize(m_oCompareSettings, tlhs, trhs);
	}

	template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(detail::isCompatTypeForAString<Tlhs>() && detail::isCompatTypeForAString<Trhs>())>>
	constexpr decltype(auto) choice_Compare(const Tlhs& tlhs, const Trhs& trhs, vlr::util::choice<0>&&) const
	{
		return detail::CompareAsAStringCompat(m_oCompareSettings, tlhs, trhs);
	}
	template<typename Tlhs, typename Trhs, typename = std::enable_if_t<(detail::isCompatTypeForWString<Tlhs>() && detail::isCompatTypeForWString<Trhs>())>>
	constexpr decltype(auto) choice_Compare(const Tlhs& tlhs, const Trhs& trhs, vlr::util::choice<1>&&) const
	{
		return detail::CompareAsWStringCompat(m_oCompareSettings, tlhs, trhs);
	}
	// No direct compare possible as this point; some conversion will be necessary
	template<typename Tlhs, typename Trhs>
	constexpr decltype(auto) choice_Compare(const Tlhs& tlhs, const Trhs& trhs, vlr::util::choice<2>&&) const
	{
		return detail::CompareDifferentCharSize(m_oCompareSettings, tlhs, trhs);
	}

public:
	template<typename TString>
	constexpr decltype(auto) IsBlank(const TString& tValue) const
	{
		return choice_IsBlank(tValue, vlr::util::choice<0>{});
	}
	template<typename Tlhs, typename Trhs>
	constexpr decltype(auto) AreEqual(const Tlhs& tlhs, const Trhs& trhs) const
	{
		return choice_AreEqual(tlhs, trhs, vlr::util::choice<0>{});
	}
	template<typename Tlhs, typename Trhs>
	constexpr decltype(auto) Compare(const Tlhs& tlhs, const Trhs& trhs) const
	{
		return choice_Compare(tlhs, trhs, vlr::util::choice<0>{});
	}
	template<typename TString, typename TPrefix>
	constexpr decltype(auto) StringHasPrefix(const TString& tString, const TPrefix& tPrefix) const
	{
		const auto& tString_ViewCompat = asStringViewCompatType(tString);
		const auto& svzString = static_cast<string_view_compat_t<TString>>(tString_ViewCompat);
		const auto& tPrefix_ViewCompat = asStringViewCompatType(tPrefix);
		const auto& svzPrefix = static_cast<string_view_compat_t<TPrefix>>(tPrefix_ViewCompat);

		if (svzString.size() <= svzPrefix.size())
		{
			return false;
		}
		const auto& svzPrefixOfString = svzString.substr(0, svzPrefix.size());

		return AreEqual(svzPrefixOfString, svzPrefix);
	}
	template<typename TString, typename TPostfix>
	constexpr decltype(auto) StringHasPostfix(const TString& tString, const TPostfix& tPostfix) const
	{
		const auto& tString_ViewCompat = asStringViewCompatType(tString);
		const auto& svzString = static_cast<string_view_compat_t<TString>>(tString_ViewCompat);
		const auto& tPostfix_ViewCompat = asStringViewCompatType(tPostfix);
		const auto& svzPostfix = static_cast<string_view_compat_t<TPostfix>>(tPostfix_ViewCompat);

		if (svzString.size() <= svzPostfix.size())
		{
			return false;
		}
		const auto& nStartIndex = svzString.size() - svzPostfix.size();
		const auto& svzPostfixOfString = svzString.substr(nStartIndex, svzPostfix.size());

		return AreEqual(svzPostfixOfString, svzPostfix);
	}

public:
	constexpr CComparator() = default;
	constexpr CComparator(const CompareSettings&& oCompareSettings)
		: m_oCompareSettings{ oCompareSettings }
	{}
};

constexpr decltype(auto) CS()
{
	//static const auto oComparator = []
	//{
	//	auto oCmparator = CComparator{ CompareSettings::ForCaseSensitive() };
	//	return oCmparator;
	//}();
	//return oComparator;
	return CComparator{ CompareSettings::ForCaseSensitive() };
}

constexpr decltype(auto) CI()
{
	//static const auto oComparator = []
	//{
	//	auto oCmparator = CComparator{ CompareSettings::ForCaseInsensitive() };
	//	return oCmparator;
	//}();
	//return oComparator;
	return CComparator{ CompareSettings::ForCaseInsensitive() };
}

// Note: Simple structs here for use in std containers (for example)
// eg: std::map<std::string, std::string, StringCompare::asCaseInsensitive>
// Note that CComparator is more optimal than doing a conversion to compare (by design)

struct asCaseSensitive
{
	template<typename Tlhs, typename Trhs>
	constexpr bool operator()(const Tlhs& tlhs, const Trhs& trhs) const
	{
		return (CS().Compare(tlhs, trhs) < 0);
	}
};

struct asCaseInsensitive
{
	template<typename Tlhs, typename Trhs>
	constexpr bool operator()(const Tlhs& tlhs, const Trhs& trhs) const
	{
		return (CI().Compare(tlhs, trhs) < 0);
	}
};

VLR_NAMESPACE_END //(StringCompare)

VLR_NAMESPACE_END //(vlr)
