#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

#include "zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( assert )

void HandleCheckFailure( vlr::tzstring_view svzFailureMessage );

#ifndef VLR_CONFIG_HAVEIMPL_HandleCheckFailure

inline void HandleCheckFailure( vlr::tzstring_view /*svzFailureMessage*/ )
{
}

#endif

NAMESPACE_END //( assert )

NAMESPACE_END //( vlr )
