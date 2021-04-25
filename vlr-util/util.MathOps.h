#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

constexpr auto ToLONGLONG( DWORD nValueLow, DWORD nValueHigh )
-> LONGLONG
{
	auto nValue = static_cast<LONGLONG>(nValueLow);
	nValue += static_cast<LONGLONG>(nValueHigh) * (LONGLONG{ MAXDWORD } + 1);
	return nValue;
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
