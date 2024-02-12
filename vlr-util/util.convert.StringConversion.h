#pragma once

#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.range_checked_cast.h"
#include "util.Unicode.h"
#include "util.types.h"

// Note on StringConversionOptions:
// This is passed through to the string width conversion calls, IFF they are called. For same-width functions,
// this parameter is ignored. There is (currently) no ASCII (system code page) -> UTF8 direct conversion, 
// for example (must go CP_ACP -> UTF16 -> UTF8).

namespace vlr {

namespace util {

namespace Convert {

// std::string <- std::string_view

inline auto ToStdStringA(std::string_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return std::string{ svValue };
}

inline auto ToStdStringA(std::wstring_view svValue, const StringConversionOptions & oConversionOptions = {})
{
#ifdef WIN32
	return CStringConversion{}.Inline_UTF16_to_MultiByte_StdString(svValue, oConversionOptions);
#elif defined(VLR_FALLBACK_Inline_UTF16_to_MultiByte_StdString)
	return VLR_FALLBACK_Inline_UTF16_to_MultiByte_StdString(svValue, oConversionOptions);
#else
	static_assert();
#endif
}

inline auto ToStdStringW(std::string_view svValue, const StringConversionOptions& oConversionOptions = {})
{
#ifdef WIN32
	return CStringConversion{}.Inline_MultiByte_to_UTF16_StdString(svValue, oConversionOptions);
#elif defined(VLR_FALLBACK_Inline_UTF16_to_MultiByte_StdString)
	return VLR_FALLBACK_Inline_MultiByte_to_UTF16_StdString(svValue, oConversionOptions);
#else
	static_assert();
#endif
}

inline auto ToStdStringW(std::wstring_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return std::wstring{ svValue };
}

// std::string <- std::string

inline decltype(auto) ToStdStringA(const std::string& sValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return sValue;
}

inline auto ToStdStringA(const std::wstring& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ sValue };
	return ToStdStringA(svValue, oConversionOptions);
}

inline auto ToStdStringW(const std::string& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ sValue };
	return ToStdStringW(svValue, oConversionOptions);
}

inline decltype(auto) ToStdStringW(const std::wstring& sValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return sValue;
}

// std::string <- CString

#if VLR_CONFIG_INCLUDE_ATL_CSTRING

inline auto ToStdStringA(const CStringA& saValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return std::string{ svValue };
}

inline auto ToStdStringA(const CStringW& swValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return ToStdStringA(svValue, oConversionOptions);
}

inline auto ToStdStringW(const CStringA& saValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return ToStdStringW(svValue, oConversionOptions);
}

inline auto ToStdStringW(const CStringW& swValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return std::wstring{ svValue };
}

#endif // VLR_CONFIG_INCLUDE_ATL_CSTRING

namespace detail {

// Convertible from "width matching" string type; special case for type with operator conversion
// Note: Necessary to resolve potential ambiguity in conversions (otherwise may match other methods instead;
// prefer operator when available to defer to potential optimized call)

template< typename TString >
inline auto ToStdStringA_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<0>&&)
-> decltype(std::declval<TString>().operator std::string())
{
	return tString.operator std::string();
}
template< typename TString  >
inline auto ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<0>&&)
-> decltype(std::declval<TString>().operator std::wstring())
{
	return tString.operator std::wstring();
}

// Convert from "width matching" string type (should include views of matching char width)

template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::string>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<1>&&)
{
	return static_cast<std::string>(tString);
}
template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::wstring>>* = nullptr  >
inline decltype(auto) ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<1>&&)
{
	return static_cast<std::wstring>(tString);
}

// Convertible from "width mismatch" string view

template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::wstring_view>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<2>&&)
{
	return ToStdStringA(static_cast<std::wstring_view>(tString), oConversionOptions);
}
template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::string_view>>* = nullptr  >
inline decltype(auto) ToStdStringW_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<2>&&)
{
	return ToStdStringW(static_cast<std::string_view>(tString), oConversionOptions);
}

// Special case for "width matching" zstring_view, since conversion may require explicit call to work with subclass

template< typename TString, typename std::enable_if_t<std::is_same_v<TString, vlr::zstring_view>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<3>&&)
{
	return tString.toStdString();
}
template< typename TString, typename std::enable_if_t<std::is_same_v<TString, vlr::wzstring_view>>* = nullptr >
inline decltype(auto) ToStdStringW_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<3>&&)
{
	return tString.toStdString();
}

// Do not know how to convert any other types; static assert if we fall through

template< typename TString >
inline decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<4>&&)
{
	VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled conversion type");
}
template< typename TString >
inline decltype(auto) ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<4>&&)
{
	VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled conversion type");
}

} // namespace detail

template< typename TString >
inline decltype(auto) ToStdStringA(const TString& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::ToStdStringA_choice(tString, oConversionOptions, vlr::util::choice<0>{});
}
template< typename TString >
inline decltype(auto) ToStdStringW(const TString& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::ToStdStringW_choice(tString, oConversionOptions, vlr::util::choice<0>{});
}

// Generic version for "StdStringT", based on compilation type
// (passes any additional arguments through)

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
		VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled character size");
	}
}

// Special-case method to convert from system default ACSCII code page
// Note: This only works when converting from narrow-width to UTF16; pass through width will ignore flag.

template< typename TString >
inline decltype(auto) ToStdStringW_FromSystemDefaultASCII(const TString& tString)
{
	return detail::ToStdStringW_choice(tString, StringConversionOptions::ForSystemDefaultASCIIIngress(), vlr::util::choice<0>{});
}

// CString[A/W] <- std::string_view

#if VLR_CONFIG_INCLUDE_ATL_CSTRING

inline decltype(auto) ToCStringA(std::string_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return CStringA{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

inline decltype(auto) ToCStringA(std::wstring_view svValue, const StringConversionOptions& oConversionOptions = {})
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_CString(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringW(std::string_view svValue, const StringConversionOptions& oConversionOptions = {})
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_CString(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringW(std::wstring_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return CStringW{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

// CString[A/W] <- CString[A/W]

inline decltype(auto) ToCStringA(const CStringA& sValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return sValue;
}

inline decltype(auto) ToCStringA(const CStringW& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ sValue.GetString(), util::range_checked_cast<size_t>(sValue.GetLength()) };
	return ToCStringA(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringW(const CStringA& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ sValue.GetString(), util::range_checked_cast<size_t>(sValue.GetLength()) };
	return ToCStringW(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringW(const CStringW& sValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return sValue;
}

// CString[A/W] <- std::string

inline decltype(auto) ToCStringA(const std::string& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ sValue };
	return ToCStringA(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringA(const std::wstring& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringA(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringW(const std::string& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ sValue };
	return ToCStringW(svValue, oConversionOptions);
}

inline decltype(auto) ToCStringW(const std::wstring& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringW(svValue, oConversionOptions);
}

// Generic version for "CStringT", based on compilation type
// (passes any additional arguments through)

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
		VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled character size");
	}
}

#endif // VLR_CONFIG_INCLUDE_ATL_CSTRING

} // namespace Convert

} // namespace util

} // namespace vlr
