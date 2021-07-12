#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

constexpr auto ToLONGLONG( DWORD nValueLow, DWORD nValueHigh )
-> LONGLONG
{
	auto nValue = static_cast<LONGLONG>(nValueLow);
	nValue += static_cast<LONGLONG>(nValueHigh) * (LONGLONG{ MAXDWORD } + 1);
	return nValue;
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
