#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

#include "include.fmt.h"

NAMESPACE_BEGIN( vlr )

template< typename TResult, typename TSource >
inline auto ConvertTo( const TSource& tSource )
{
	return TResult{ tSource };
}

template< typename TResult >
inline auto ConvertTo( const std::string& tSource )
{
	return TResult{ tSource.c_str() };
}

template< typename TResult >
inline auto ConvertTo( const std::wstring& tSource )
{
	return TResult{ tSource.c_str() };
}

NAMESPACE_BEGIN( detail )

#if VLR_CONFIG_INCLUDE_AFX

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

#endif // VLR_CONFIG_INCLUDE_AFX

NAMESPACE_END //( detail )

#if VLR_CONFIG_INCLUDE_AFX

template< typename TResult, typename... Arg >
inline auto formatpf_to( lib_fmt::FormatStringA svFormatString, Arg&&... args )
-> std::enable_if_t<std::is_same_v<TResult, CStringA>, CStringA>
{
	return detail::formatpf_to_TResult<TResult, lib_fmt::FormatStringA>( svFormatString, std::forward<Arg>( args )... );
}

template< typename TResult, typename... Arg >
inline auto formatpf_to( lib_fmt::FormatStringW svFormatString, Arg&&... args )
-> std::enable_if_t<std::is_same_v<TResult, CStringW>, CStringW>
{
	return detail::formatpf_to_TResult<TResult, lib_fmt::FormatStringW>( svFormatString, std::forward<Arg>( args )... );
}

#endif // VLR_CONFIG_INCLUDE_AFX

NAMESPACE_END //( vlr )
