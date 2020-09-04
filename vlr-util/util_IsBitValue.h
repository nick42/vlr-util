#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

template< typename TNumericType >
constexpr bool IsSingleBitValue( TNumericType nValue )
{
	if (nValue == 0)
	{
		return false;
	}
	if (nValue == 1)
	{
		return true;
	}
	// If the low bit is set, and it's not the only bit set, then the value isn't a bit value
	if ((nValue & 0x01) == 0x01)
	{
		return false;
	}

	return IsSingleBitValue( nValue >> 1 );
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
