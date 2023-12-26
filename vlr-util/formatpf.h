#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "include.fmt.h"
#include "util.choice.h"
#include "util.convert.StringConversion.h"
#include "util.std_aliases.h"

namespace vlr {

namespace detail {

// The pass-through case

template< typename TResult, typename TSource, typename std::enable_if_t<std::is_same_v<std::decay_t<TResult>, std::decay_t<TSource>>>* = nullptr >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<0>&& )
{
	return tSource;
}

// The direct convertible case

template< typename TResult, typename TSource, typename std::enable_if_t<std::is_convertible_v<TSource, TResult>>* = nullptr >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<1>&& )
{
	return static_cast<TResult>(tSource);
}

// The ToStdString case(s)

template< typename TResult, typename TSource, typename std::enable_if_t<std::is_same_v<std::decay_t<TResult>, std::string>>* = nullptr >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<2>&& )
{
	return util::Convert::ToStdStringA( tSource );
}

template< typename TResult, typename TSource, typename std::enable_if_t<std::is_same_v<std::decay_t<TResult>, std::wstring>>* = nullptr >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<3>&& )
{
	return util::Convert::ToStdStringW( tSource );
}

// The ToCString case(s)

template< typename TResult, typename TSource, typename std::enable_if_t<std::is_same_v<std::decay_t<TResult>, CStringA>>* = nullptr >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<4>&& )
{
	return util::Convert::ToCStringA( tSource );
}

template< typename TResult, typename TSource, typename std::enable_if_t<std::is_same_v<std::decay_t<TResult>, CStringW>>* = nullptr >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<5>&& )
{
	return util::Convert::ToCStringW( tSource );
}

// The fallthrough case

template< typename TResult, typename TSource >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<6>&& )
{
	return TResult{ tSource };
}

} // namespace detail

template< typename TResult, typename TSource >
inline auto ConvertTo( const TSource& tSource )
{
	return detail::ConvertTo_choice<TResult>( tSource, vlr::util::choice<0>{} );
}

namespace detail {

#if VLR_CONFIG_INCLUDE_ATL_CSTRING

template< typename TResult, typename TFormatString, typename... Arg >
inline auto formatpf_to_TResult( TFormatString svFormatString, Arg&&... args )
{
	try
	{
		auto sResult = fmt::sprintf( svFormatString, std::forward<Arg>( args )... );
		return ConvertTo<TResult>( sResult );
	}
	catch (const fmt::format_error& /*oError*/)
	{
		throw;
	}

	// If we didn't throw out of the catch, return an empty result value
	return TResult{};
}

//template< typename TResult, typename TFormatString >
//inline auto vformatpf_to_TResult( TFormatString svFormatString, const fmt::format_args_t<fmt::printf_context>& oArgs )
//{
//	try
//	{
//		auto sResult = fmt::vsprintf( svFormatString, oArgs );
//		return ConvertTo<TResult>( sResult );
//	}
//	catch (const fmt::format_error& /*oError*/)
//	{
//		throw;
//	}
//
//	// If we didn't throw out of the catch, return an empty result value
//	return TResult{};
//}

#endif // VLR_CONFIG_INCLUDE_ATL_CSTRING

} // namespace detail

template< typename TResult, typename... Arg >
inline auto formatpf_to( lib_fmt::FormatStringA svFormatString, Arg&&... args )
-> TResult
{
	return detail::formatpf_to_TResult<TResult, lib_fmt::FormatStringA>( svFormatString, std::forward<Arg>( args )... );
}

template< typename TResult, typename... Arg >
inline auto formatpf_to( lib_fmt::FormatStringW svFormatString, Arg&&... args )
-> TResult
{
	return detail::formatpf_to_TResult<TResult, lib_fmt::FormatStringW>( svFormatString, std::forward<Arg>( args )... );
}

template< typename... Arg >
inline auto formatpf( lib_fmt::FormatStringA svFormatString, Arg&&... args )
{
	return detail::formatpf_to_TResult<vlr::string, lib_fmt::FormatStringA>( svFormatString, std::forward<Arg>( args )... );
}

template< typename... Arg >
inline auto formatpf( lib_fmt::FormatStringW svFormatString, Arg&&... args )
{
	return detail::formatpf_to_TResult<vlr::tstring, lib_fmt::FormatStringW>( svFormatString, std::forward<Arg>( args )... );
}

} // namespace vlr
