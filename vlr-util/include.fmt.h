#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#define FMT_HEADER_ONLY

#include <fmt/format.h>
#include <fmt/printf.h>

// Note: Some using aliases to allow us to specify the types which the fmt library expects 
// for various method calls.

NAMESPACE_BEGIN( lib_fmt )

using FormatStringA = fmt::basic_string_view<char>;
using FormatStringW = fmt::basic_string_view<wchar_t>;
using FormatStringT = fmt::basic_string_view<TCHAR>;

NAMESPACE_END //( lib_fmt )

#if VLR_CONFIG_INCLUDE_AFX

// Note: If you have "custom" types which you want to pass as format parameters, you need to 
// create template specializations which tell the library how to parse and format them.
// This code essentially just converts CString to string_view, which is supported directly.
// As of time of writing: https://fmt.dev/latest/api.html#formatting-user-defined-types

#include <afxstr.h>

#include "zstring_view.h"

template<>
struct fmt::formatter<CStringA>
	: fmt::formatter<string_view>
{
	//constexpr auto parse( format_parse_context& ctx )
	//{
	//	return ctx.begin();
	//}
	template< typename ParseContext >
	auto format( const CStringA& sParam, ParseContext& ctx )
	{
		return fmt::formatter<string_view>::format_to( ctx.begin(), "{}", vlr::zstring_view{ sParam } );
	}
};

template<>
struct fmt::formatter<CStringW>
	: fmt::formatter<wstring_view>
{
	//constexpr auto parse( format_parse_context& ctx )
	//{
	//	return ctx.begin();
	//}
	template< typename ParseContext >
	auto format( const CStringW& sParam, ParseContext& ctx )
	{
		return fmt::formatter<wstring_view>::format_to( ctx.begin(), "{}", vlr::wzstring_view{ sParam } );
	}
};

#endif
