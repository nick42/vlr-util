#pragma once

#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.range_checked_cast.h"
#include "util.Unicode.h"
#include "util.types.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(util)

VLR_NAMESPACE_BEGIN(Convert)

// std::string <- std::string_view

inline auto ToStdStringA(std::string_view svValue)
{
	return std::string{ svValue };
}

inline auto ToStdStringA(std::wstring_view svValue)
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_StdString(svValue);
}

inline auto ToStdStringW(std::string_view svValue)
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_StdString(svValue);
}

inline auto ToStdStringW(std::wstring_view svValue)
{
	return std::wstring{ svValue };
}

// std::string <- std::string

inline decltype(auto) ToStdStringA(const std::string& sValue)
{
	return sValue;
}

inline auto ToStdStringA(const std::wstring& sValue)
{
	auto svValue = std::wstring_view{ sValue };
	return ToStdStringA(svValue);
}

inline auto ToStdStringW(const std::string& sValue)
{
	auto svValue = std::string_view{ sValue };
	return ToStdStringW(svValue);
}

inline decltype(auto) ToStdStringW(const std::wstring& sValue)
{
	return sValue;
}

// std::string <- CString

inline auto ToStdStringA(const CStringA& saValue)
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return std::string{ svValue };
}

inline auto ToStdStringA(const CStringW& swValue)
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return ToStdStringA(svValue);
}

inline auto ToStdStringW(const CStringA& saValue)
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return ToStdStringW(svValue);
}

inline auto ToStdStringW(const CStringW& swValue)
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return std::wstring{ svValue };
}

VLR_NAMESPACE_BEGIN(detail)

// Convertible to "matching" width string type; special case for type with operator conversion
// Note: Necessary to resolve potential ambiguity in conversions

template< typename TString >
inline auto ToStdStringA_choice(const TString& tString, vlr::util::choice<0>&&)
-> decltype(std::declval<TString>().operator std::string())
{
	return tString.operator std::string();
}
template< typename TString  >
inline auto ToStdStringW_choice(const TString& tString, vlr::util::choice<0>&&)
-> decltype(std::declval<TString>().operator std::wstring())
{
	return tString.operator std::wstring();
}

// Convertible to "matching" width string type (should include views of matching char width)

template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::string>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, vlr::util::choice<1>&&)
{
	return static_cast<std::string>(tString);
}
template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::wstring>>* = nullptr  >
inline decltype(auto) ToStdStringW_choice(const TString& tString, vlr::util::choice<1>&&)
{
	return static_cast<std::wstring>(tString);
}

// Convertible to "opposite" width string view

template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::wstring_view>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, vlr::util::choice<2>&&)
{
	return ToStdStringA(static_cast<std::wstring_view>(tString));
}
template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::string_view>>* = nullptr  >
inline decltype(auto) ToStdStringW_choice(const TString& tString, vlr::util::choice<2>&&)
{
	return ToStdStringW(static_cast<std::string_view>(tString));
}

// Do not know how to convert this type

template< typename TString >
inline decltype(auto) ToStdStringA_choice(const TString& tString, vlr::util::choice<3>&&)
{
	static_assert(VLR_DEPENDENT_FALSE, "Unhandled conversion type");
}
template< typename TString >
inline decltype(auto) ToStdStringW_choice(const TString& tString, vlr::util::choice<3>&&)
{
	static_assert(VLR_DEPENDENT_FALSE, "Unhandled conversion type");
}

VLR_NAMESPACE_END //( detail )

template< typename TString >
inline decltype(auto) ToStdStringA(const TString& tString)
{
	return detail::ToStdStringA_choice(tString, vlr::util::choice<0>{});
}
template< typename TString >
inline decltype(auto) ToStdStringW(const TString& tString)
{
	return detail::ToStdStringW_choice(tString, vlr::util::choice<0>{});
}

// Generic version for "StdStringT", based on compilation type

template< typename TString, typename... Arg >
inline decltype(auto) ToStdString(const TString& tString, Arg&&... args)
{
	if constexpr (vlr::ModuleContext::Compilation::DefaultCharTypeIs_char())
	{
		return ToStdStringA(tString, std::forward<Arg>(args)...);
	}
	else if constexpr (vlr::ModuleContext::Compilation::DefaultCharTypeIs_wchar_t())
	{
		return ToStdStringW(tString, std::forward<Arg>(args)...);
	}
	else
	{
		static_assert(VLR_DEPENDENT_FALSE, "Unhandled character size");
	}
}

// std::string <- std::string_view

inline decltype(auto) ToCStringA(std::string_view svValue)
{
	return CStringA{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

inline decltype(auto) ToCStringA(std::wstring_view svValue)
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_CString(svValue);
}

inline decltype(auto) ToCStringW(std::string_view svValue)
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_CString(svValue);
}

inline decltype(auto) ToCStringW(std::wstring_view svValue)
{
	return CStringW{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

// CString <- CString

inline decltype(auto) ToCStringA(const CStringA& sValue)
{
	return sValue;
}

inline decltype(auto) ToCStringA(const CStringW& sValue)
{
	auto svValue = std::wstring_view{ sValue.GetString(), util::range_checked_cast<size_t>(sValue.GetLength()) };
	return ToCStringA(svValue);
}

inline decltype(auto) ToCStringW(const CStringA& sValue)
{
	auto svValue = std::string_view{ sValue.GetString(), util::range_checked_cast<size_t>(sValue.GetLength()) };
	return ToCStringW(svValue);
}

inline decltype(auto) ToCStringW(const CStringW& sValue)
{
	return sValue;
}

// CString <- std::string

inline decltype(auto) ToCStringA(const std::string& sValue)
{
	auto svValue = std::string_view{ sValue };
	return ToCStringA(svValue);
}

inline decltype(auto) ToCStringA(const std::wstring& sValue)
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringA(svValue);
}

inline decltype(auto) ToCStringW(const std::string& sValue)
{
	auto svValue = std::string_view{ sValue };
	return ToCStringW(svValue);
}

inline decltype(auto) ToCStringW(const std::wstring& sValue)
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringW(svValue);
}

// Generic version for "CStringT", based on compilation type

template< typename TString, typename... Arg >
inline decltype(auto) ToCString( const TString& tString, Arg&&... args )
{
	if constexpr (vlr::ModuleContext::Compilation::DefaultCharTypeIs_char())
	{
		return ToCStringA(tString, std::forward<Arg>(args)...);
	}
	else if constexpr (vlr::ModuleContext::Compilation::DefaultCharTypeIs_wchar_t())
	{
		return ToCStringW(tString, std::forward<Arg>(args)...);
	}
	else
	{
		static_assert(VLR_DEPENDENT_FALSE, "Unhandled character size");
	}
}

VLR_NAMESPACE_END //( Convert )

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
