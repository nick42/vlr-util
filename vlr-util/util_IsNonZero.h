#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

template< typename TValue >
constexpr bool IsNonZero( const TValue& tValue )
{
	return (!!tValue);
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
