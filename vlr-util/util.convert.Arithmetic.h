#pragma once

#include <stdint.h>

#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.range_checked_cast.h"

namespace vlr {

namespace util {

namespace Convert {

auto To_unit64( ULARGE_INTEGER nValue )
{
	return static_cast<uint64_t>(nValue.QuadPart);
}

} // namespace Convert

} // namespace util

} // namespace vlr
