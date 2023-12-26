#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

namespace util {

template< typename TValue, typename TBitValue >
constexpr auto IsBitSet( TValue nValue, TBitValue nBitValue )
{
	auto&& tBitValueAsValue = static_cast<TValue>(nBitValue);
	return ((nValue & tBitValueAsValue) == tBitValueAsValue);
}

} // namespace util

} // namespace vlr
