#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#ifdef WIN32
#include "util.types.win32.h"
#endif
#ifdef __linux__
#include "util.types.linux.h"
#endif

VLR_NAMESPACE_BEGIN( vlr )

#ifndef _HRESULT_DEFINED
#pragma error("HRESULT not defined?")
#endif

using ResultCode = HRESULT;

VLR_NAMESPACE_END //( vlr )
