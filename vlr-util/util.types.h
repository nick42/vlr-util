#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"

#ifdef WIN32
#include "util.types.win32.h"
#endif
#ifdef POSIX
#include "util.types.linux.h"
#endif

VLR_NAMESPACE_BEGIN( vlr )

// Note: This good workaround suggested by MS, for conformance updates related to static_assert
// See: https://docs.microsoft.com/en-us/cpp/overview/cpp-conformance-improvements?view=msvc-170

template<typename>
constexpr bool dependent_false = false;

using ResultCode = HRESULT;

VLR_NAMESPACE_END //( vlr )
