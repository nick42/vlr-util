#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

namespace util {

constexpr auto ToLONGLONG( DWORD nValueLow, DWORD nValueHigh )
-> LONGLONG
{
	auto nValue = static_cast<LONGLONG>(nValueLow);
	nValue += static_cast<LONGLONG>(nValueHigh) * (LONGLONG{ MAXDWORD } + 1);
	return nValue;
}

} // namespace util

} // namespace vlr
