#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#ifdef WIN32
#include <ATLComTime.h>
#endif

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( types )

#ifdef WIN32
using DateTime = ATL::COleDateTime;
using DateTimeSpan = ATL::COleDateTimeSpan;
#endif

VLR_NAMESPACE_END //( types )

VLR_NAMESPACE_END //( vlr )
