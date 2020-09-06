#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "include.fmt.h"
#include "vlr/util.choice.h"
#include "vlr/util.convert.StringConversion.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( detail )

// The pass-through case

template< typename TResult, typename TSource, typename = std::enable_if_t<std::is_same_v<std::decay_t<TResult>, std::decay_t<TSource>>> >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<0>&& )
{
	return tSource;
}

// The direct convertible case

template< typename TResult, typename TSource, typename = std::enable_if_t<std::is_convertible_v<TSource, TResult>> >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<1>&& )
{
	return static_cast<TResult>(tSource);
}

// The ToStdString case(s)

template< typename TResult, typename TSource, typename = std::enable_if_t<std::is_same_v<std::decay_t<TResult>, std::string>> >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<2>&& )
{
	return util::Convert::ToStdStringA( tSource );
}

template< typename TResult, typename TSource, typename = std::enable_if_t<std::is_same_v<std::decay_t<TResult>, std::wstring>> >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<3>&& )
{
	return util::Convert::ToStdStringW( tSource );
}

// The ToCString case(s)

template< typename TResult, typename TSource, typename = std::enable_if_t<std::is_same_v<std::decay_t<TResult>, CStringA>> >
inline decltype(auto) ConvertTo_choice( const TSource& tSource, vlr::util::choice<4>&& )
{
	return util::Convert::ToCStringA( tSource );
}

template< typename TResult, typename TSource, typename = std::enable_if_t<std::is_same_v<std::decay_t<TResult>, CStringW>> >
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

NAMESPACE_END //( detail )

template< typename TResult, typename TSource >
inline auto ConvertTo( const TSource& tSource )
{
	return detail::ConvertTo_choice<TResult>( tSource, vlr::util::choice<0>{} );
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

NAMESPACE_END //( vlr )
