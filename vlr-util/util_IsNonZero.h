#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

#include "util_choice.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( detail )

template< typename TValue >
constexpr auto IsNonZero_choice( const TValue& tValue, choice<0>&& )
-> std::enable_if_t<std::is_convertible_v<TValue, bool>, bool>
{
	return static_cast<bool>(tValue);
}

template< typename TValue >
constexpr auto IsNonZero_choice( const TValue& tValue, choice<1>&& )
-> decltype(tValue != 0)
{
	return (tValue != 0);
}

template< typename TValue >
constexpr auto IsNonZero_choice( const TValue& tValue, choice<2>&& )
-> decltype(tValue != nullptr)
{
	return (tValue != nullptr);
}

NAMESPACE_END //( detail )

template< typename TValue >
constexpr bool IsNonZero( const TValue& tValue )
{
	return detail::IsNonZero_choice( tValue, choice<0>{} );
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
