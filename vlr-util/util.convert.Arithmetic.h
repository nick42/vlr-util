#pragma once

#include <stdint.h>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#include "util.range_checked_cast.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

VLR_NAMESPACE_BEGIN( Convert )

auto To_unit64( ULARGE_INTEGER nValue )
{
	return static_cast<uint64_t>(nValue.QuadPart);
}

VLR_NAMESPACE_END //( Convert )

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
