#pragma once

#include <type_traits>

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

// Windows has a historical type define for BOOL as int. We do not want to define/use this directly, as
// other platform compilers (eg: Xcode) may define BOOL as bool, and thus get a different type. When 
// making method to be compatible with Win32 (in this library), use this type instead.
using WIN_BOOL = int;

} // namespace vlr
