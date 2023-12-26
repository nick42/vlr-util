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

namespace vlr {

#ifndef _HRESULT_DEFINED
#pragma error("HRESULT not defined?")
#endif

using ResultCode = HRESULT;

} // namespace vlr
