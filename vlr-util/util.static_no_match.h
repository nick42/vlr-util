#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

template< bool flag = false >
void static_no_match()
{
	static_assert(flag, "no match");
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )

