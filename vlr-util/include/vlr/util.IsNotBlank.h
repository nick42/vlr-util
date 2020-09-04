#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.choice.h"
#include "zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( detail )

// Note: Using our type, to allow more implicit construction from string classes

template< typename TValue, typename = std::enable_if_t<std::is_constructible_v<vlr::zstring_view, const TValue&>> >
constexpr bool IsNotBlank_choice( const TValue& tValue, choice<0>&& )
{
	return (vlr::zstring_view{ tValue }.length() > 0);
}

template< typename TValue, typename = std::enable_if_t<std::is_constructible_v<vlr::wzstring_view, const TValue&>> >
constexpr bool IsNotBlank_choice( const TValue& tValue, choice<1>&& )
{
	return (vlr::wzstring_view{ tValue }.length() > 0);
}

//template< typename TValue, typename TFallbackChoice >
//constexpr bool IsNotBlank( const TValue& tValue, TFallbackChoice&& )
//{
//	return (tValue != "");
//}

NAMESPACE_END //( detail )

constexpr bool IsNotBlank( vlr::zstring_view svzValue )
{
	return (svzValue.length() > 0);
}

constexpr bool IsNotBlank( vlr::wzstring_view svzValue )
{
	return (svzValue.length() > 0);
}

constexpr bool IsNotBlank( std::string_view svValue )
{
	return (svValue.length() > 0);
}

constexpr bool IsNotBlank( std::wstring_view svValue )
{
	return (svValue.length() > 0);
}

template< typename TValue >
constexpr bool IsNotBlank( const TValue& tValue )
{
	return detail::IsNotBlank_choice( tValue, choice<0>{} );
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
