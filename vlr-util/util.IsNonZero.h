#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.choice.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

VLR_NAMESPACE_BEGIN( detail )

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
constexpr auto IsNonZero_choice( const TValue* tValue, choice<2>&& )
-> decltype(tValue != nullptr)
{
	return (tValue != nullptr);
}

VLR_NAMESPACE_END //( detail )

template< typename TValue >
constexpr bool IsNonZero( const TValue& tValue )
{
	return detail::IsNonZero_choice( tValue, choice<0>{} );
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
