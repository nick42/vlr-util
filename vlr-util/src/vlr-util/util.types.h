#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#ifdef WIN32
#include <ATLComTime.h>
#endif

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( types )

#ifdef WIN32
using DateTime = ATL::COleDateTime;
using DateTimeSpan = ATL::COleDateTimeSpan;
#endif

NAMESPACE_END //( types )

NAMESPACE_END //( vlr )
