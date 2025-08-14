#pragma once

#include <string>

#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.range_checked_cast.h"
#include "util.StringConversion.h"
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
// Note: noexcept IFF the std::string constructor is noexcept.
// Note that this is not currently the specification, as of C++20, but may be so in the future.
noexcept(noexcept(std::string{svValue}))
{
	return std::string{ svValue };
}

inline auto ToStdStringA(std::wstring_view svValue, const StringConversionOptions& oConversionOptions = {})
noexcept(noexcept(CStringConversion{}.Inline_UTF16_to_MultiByte_StdString(svValue, oConversionOptions)))
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_StdString(svValue, oConversionOptions);
}

inline auto ToStdStringW(std::string_view svValue, const StringConversionOptions& oConversionOptions = {})
noexcept(noexcept(CStringConversion{}.Inline_MultiByte_to_UTF16_StdString(svValue, oConversionOptions)))
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_StdString(svValue, oConversionOptions);
}

inline auto ToStdStringW(std::wstring_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
// Note: noexcept IFF the std::wstring constructor is noexcept.
// Note that this is not currently the specification, as of C++20, but may be so in the future.
noexcept(noexcept(std::wstring{ svValue }))
{
	return std::wstring{ svValue };
}

// std::string <- std::string

constexpr decltype(auto) ToStdStringA(const std::string& sValue, const StringConversionOptions& /*oConversionOptions*/ = {}) noexcept
{
	return (sValue);
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

constexpr decltype(auto) ToStdStringW(const std::wstring& sValue, const StringConversionOptions& /*oConversionOptions*/ = {}) noexcept
{
	return (sValue);
}

// std::string <- CString

#if VLR_CONFIG_INCLUDE_ATL_CString

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

#endif // VLR_CONFIG_INCLUDE_ATL_CString

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
inline auto ToStdStringA_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<3>&&)
{
	return tString.toStdString();
}
template< typename TString, typename std::enable_if_t<std::is_same_v<TString, vlr::wzstring_view>>* = nullptr >
inline auto ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<3>&&)
{
	return tString.toStdString();
}

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t

template< typename TString, typename std::enable_if_t<std::is_same_v<TString, _bstr_t>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<4>&&)
{
	return ToStdStringA(std::wstring{ static_cast<const wchar_t*>(tString), static_cast<std::wstring::size_type>(tString.length()) }, oConversionOptions);
}
template< typename TString, typename std::enable_if_t<std::is_same_v<TString, _bstr_t>>* = nullptr >
inline auto ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<4>&&)
{
	return std::wstring{ static_cast<const wchar_t*>(tString), static_cast<std::wstring::size_type>(tString.length()) };
}

#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR

template< typename TString, typename std::enable_if_t<std::is_same_v<TString, ATL::CComBSTR>>* = nullptr >
inline decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<4>&&)
{
	return ToStdStringA(std::wstring{ static_cast<const BSTR>(tString), static_cast<std::wstring::size_type>(tString.Length()) }, oConversionOptions);
}
template< typename TString, typename std::enable_if_t<std::is_same_v<TString, ATL::CComBSTR>>* = nullptr >
inline auto ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<4>&&)
{
	return std::wstring{ static_cast<const BSTR>(tString), static_cast<std::wstring::size_type>(tString.Length()) };
}

#endif

// Do not know how to convert any other types; static assert if we fall through

template< typename TString >
constexpr decltype(auto) ToStdStringA_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<5>&&)
{
	VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled conversion type");
}
template< typename TString >
constexpr decltype(auto) ToStdStringW_choice(const TString& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<5>&&)
{
	VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled conversion type");
}

} // namespace detail

template< typename TString >
constexpr decltype(auto) ToStdStringA(const TString& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::ToStdStringA_choice(tString, oConversionOptions, vlr::util::choice<0>{});
}
template< typename TString >
constexpr decltype(auto) ToStdStringW(const TString& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::ToStdStringW_choice(tString, oConversionOptions, vlr::util::choice<0>{});
}

// Generic version for "StdStringT", based on compilation type
// (passes any additional arguments through)

template< typename TString, typename... Arg >
constexpr decltype(auto) ToStdString(const TString& tString, Arg&&... args)
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

#if VLR_CONFIG_INCLUDE_ATL_CString

inline auto ToCStringA(std::string_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return CStringA{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

inline auto ToCStringA(std::wstring_view svValue, const StringConversionOptions& oConversionOptions = {})
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_CString(svValue, oConversionOptions);
}

inline auto ToCStringW(std::string_view svValue, const StringConversionOptions& oConversionOptions = {})
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_CString(svValue, oConversionOptions);
}

inline auto ToCStringW(std::wstring_view svValue, const StringConversionOptions& /*oConversionOptions*/ = {})
{
	return CStringW{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

// CString[A/W] <- CString[A/W]

constexpr decltype(auto) ToCStringA(const CStringA& sValue, const StringConversionOptions& /*oConversionOptions*/ = {}) noexcept
{
	return (sValue);
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

constexpr decltype(auto) ToCStringW(const CStringW& sValue, const StringConversionOptions& /*oConversionOptions*/ = {}) noexcept
{
	return (sValue);
}

// CString[A/W] <- std::string

inline auto ToCStringA(const std::string& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ sValue };
	return ToCStringA(svValue, oConversionOptions);
}

inline auto ToCStringA(const std::wstring& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringA(svValue, oConversionOptions);
}

inline auto ToCStringW(const std::string& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::string_view{ sValue };
	return ToCStringW(svValue, oConversionOptions);
}

inline auto ToCStringW(const std::wstring& sValue, const StringConversionOptions& oConversionOptions = {})
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringW(svValue, oConversionOptions);
}

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t

inline decltype(auto) ToCStringA(const _bstr_t& bsString, const StringConversionOptions& oConversionOptions = {})
{
	return ToCStringA(ToStdStringW(bsString), oConversionOptions);
}

inline decltype(auto) ToCStringW(const _bstr_t& bsString, const StringConversionOptions& oConversionOptions = {})
{
	return ToCStringW(ToStdStringW(bsString), oConversionOptions);
}

#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR

inline decltype(auto) ToCStringA(const CComBSTR& bsString, const StringConversionOptions& oConversionOptions = {})
{
	return ToCStringA(ToStdStringW(bsString), oConversionOptions);
}

inline decltype(auto) ToCStringW(const CComBSTR& bsString, const StringConversionOptions& oConversionOptions = {})
{
	return ToCStringW(ToStdStringW(bsString), oConversionOptions);
}

#endif

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

#endif // VLR_CONFIG_INCLUDE_ATL_CString

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t

namespace detail {

constexpr decltype(auto) To_bstr_t_choice(const _bstr_t& bsString, const StringConversionOptions & /*oConversionOptions*/, vlr::util::choice<0>&&) noexcept
{
	return (bsString);
}

// Note: Unfortunately, _bstr_t doesn't have a size-based constructor...

template <typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::wstring_view>>* = nullptr>
inline auto To_bstr_t_choice(const TString& tString, const StringConversionOptions & /*oConversionOptions*/, vlr::util::choice<1>&&)
{
	auto svString = static_cast<std::wstring_view>(tString);
	auto bsString = ::SysAllocStringLen(svString.data(), static_cast<UINT>(svString.size()));
	return _bstr_t{ bsString, false };
}

template <typename TString>
inline auto To_bstr_t_choice(const TString& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<2>&&)
{
	auto swString = ToStdStringW(tString, oConversionOptions);
	auto bsString = ::SysAllocStringLen(swString.data(), static_cast<UINT>(swString.length()));
	return _bstr_t{ bsString, false };
}

} // namespace detail

template <typename TString>
inline auto To_bstr_t(const TString& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::To_bstr_t_choice(tString, oConversionOptions, vlr::util::choice<0>{});
}

#endif // #if VLR_CONFIG_INCLUDE_WIN32_bstr_t

namespace detail {

// Convert from "width matching" string type (should include views of matching char width)

template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::string_view>>* = nullptr >
constexpr auto ToFmtArg_StringA_choice(TString&& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<1>&&) noexcept
{
	return static_cast<std::string_view>(tString);
}
template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, std::wstring_view>>* = nullptr  >
constexpr auto ToFmtArg_StringW_choice(TString&& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<1>&&) noexcept
{
	return static_cast<std::wstring_view>(tString);
}

template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, const char*>>* = nullptr >
constexpr auto ToFmtArg_StringA_choice(TString&& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<2>&&) noexcept
{
	return static_cast<const char*>(tString);
}
template< typename TString, typename std::enable_if_t<std::is_convertible_v<const TString&, const wchar_t*>>* = nullptr  >
constexpr auto ToFmtArg_StringW_choice(TString&& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<2>&&) noexcept
{
	return static_cast<const wchar_t*>(tString);
}

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t

template< typename TString, typename std::enable_if_t<std::is_same_v<TString, _bstr_t>>* = nullptr >
inline auto ToFmtArg_StringA_choice(TString&& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<4>&&)
{
	return ToFmtArg_StringA_choice(std::wstring{ static_cast<const wchar_t*>(tString), static_cast<std::wstring::size_type>(tString.length()) }, oConversionOptions);
}
template< typename TString, typename std::enable_if_t<std::is_same_v<TString, _bstr_t>>* = nullptr >
inline auto ToFmtArg_StringW_choice(TString&& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<4>&&)
{
	return std::wstring{ static_cast<const wchar_t*>(tString), static_cast<std::wstring::size_type>(tString.length()) };
}

#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR

template< typename TString, typename std::enable_if_t<std::is_same_v<TString, ATL::CComBSTR>>* = nullptr >
inline auto ToFmtArg_StringA_choice(TString&& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<4>&&)
{
	return ToFmtArg_StringA_choice(std::wstring{ static_cast<const BSTR>(tString), static_cast<std::wstring::size_type>(tString.Length()) }, oConversionOptions);
}
template< typename TString, typename std::enable_if_t<std::is_same_v<TString, ATL::CComBSTR>>* = nullptr >
inline auto ToFmtArg_StringW_choice(TString&& tString, const StringConversionOptions& /*oConversionOptions*/, vlr::util::choice<4>&&)
{
	return std::wstring{ static_cast<const BSTR>(tString), static_cast<std::wstring::size_type>(tString.Length()) };
}

#endif

// Fallback option is to call string conversion

template< typename TString >
constexpr decltype(auto) ToFmtArg_StringA_choice(TString&& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<9>&&)
{
	return ToStdStringA(std::forward<TString>(tString), oConversionOptions);
}
template< typename TString >
constexpr decltype(auto) ToFmtArg_StringW_choice(TString&& tString, const StringConversionOptions& oConversionOptions, vlr::util::choice<9>&&)
{
	return ToStdStringW(std::forward<TString>(tString), oConversionOptions);
}

} // namespace detail

template <typename TString>
constexpr decltype(auto) ToFmtArg_StringA(TString&& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::ToFmtArg_StringA_choice(std::forward<TString>(tString), oConversionOptions, vlr::util::choice<0>{});
}

template <typename TString>
constexpr decltype(auto) ToFmtArg_StringW(TString&& tString, const StringConversionOptions& oConversionOptions = {})
{
	return detail::ToFmtArg_StringW_choice(std::forward<TString>(tString), oConversionOptions, vlr::util::choice<0>{});
}

template <typename TString, typename... Arg>
constexpr decltype(auto) ToFmtArg_String(TString&& tString, Arg&&... args)
{
	if constexpr (vlr::ModuleContext::Compilation::DefaultCharTypeIs_char())
	{
		return ToFmtArg_StringA(std::forward<TString>(tString), std::forward<Arg>(args)...);
	}
	else if constexpr (vlr::ModuleContext::Compilation::DefaultCharTypeIs_wchar_t())
	{
		return ToFmtArg_StringW(std::forward<TString>(tString), std::forward<Arg>(args)...);
	}
	else
	{
		VLR_TYPE_DEPENDENT_STATIC_FAIL(TString, "Unhandled character size");
	}
}

} // namespace Convert

} // namespace util

} // namespace vlr
