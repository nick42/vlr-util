#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "zstring_view.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( assert )

void HandleCheckFailure( vlr::tzstring_view svzFailureMessage );

#ifndef VLR_CONFIG_HAVEIMPL_HandleCheckFailure

inline void HandleCheckFailure( vlr::tzstring_view /*svzFailureMessage*/ )
{
	ASSERT( 0 );
}

#endif

VLR_NAMESPACE_END //( assert )

VLR_NAMESPACE_END //( vlr )
