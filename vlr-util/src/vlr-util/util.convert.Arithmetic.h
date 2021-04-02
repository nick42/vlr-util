#pragma once

#include <stdint.h>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.range_checked_cast.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( Convert )

auto To_unit64( ULARGE_INTEGER nValue )
{
	return static_cast<uint64_t>(nValue.QuadPart);
}

NAMESPACE_END //( Convert )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
