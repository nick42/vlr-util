#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

template< typename TValue, typename TBitValue >
constexpr auto IsBitSet( TValue nValue, TBitValue nBitValue )
{
	auto&& tBitValueAsValue = static_cast<TValue>(nBitValue);
	return ((nValue & nBitValue) == nBitValue);
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
