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
	// Note: This works poorly when not compiling with MFC; figure out something better for default
#ifdef _MFC_VER
	ASSERT( 0 );
#endif
}

#endif

} // namespace assert

} // namespace vlr
