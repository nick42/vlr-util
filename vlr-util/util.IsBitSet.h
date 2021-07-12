#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

template< typename TValue, typename TBitValue >
constexpr auto IsBitSet( TValue nValue, TBitValue nBitValue )
{
	auto&& tBitValueAsValue = static_cast<TValue>(nBitValue);
	return ((nValue & tBitValueAsValue) == tBitValueAsValue);
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
