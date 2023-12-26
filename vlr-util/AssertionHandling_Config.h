#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "zstring_view.h"

namespace vlr {

namespace assert {

void HandleCheckFailure( vlr::tzstring_view svzFailureMessage );

#ifndef VLR_CONFIG_HAVEIMPL_HandleCheckFailure

inline void HandleCheckFailure( vlr::tzstring_view /*svzFailureMessage*/ )
{
	ASSERT( 0 );
}

#endif

} // namespace assert

} // namespace vlr
