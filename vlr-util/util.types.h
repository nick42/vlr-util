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

// Note: This good workaround suggested by MS, for conformance updates related to static_assert
// See: https://docs.microsoft.com/en-us/cpp/overview/cpp-conformance-improvements?view=msvc-170

template<typename = void>
constexpr bool dependent_false = false;

// Note: The above does not work for GCC, because it cannot deal with a template type without 
// specifying the template parameter. Unfortunately, MSVC cannot do the latter. =/
#ifdef _MSC_VER
#define VLR_DEPENDENT_FALSE dependent_false
#endif
#ifdef __GNUC__
#define VLR_DEPENDENT_FALSE dependent_false<false>
#endif

using ResultCode = HRESULT;

VLR_NAMESPACE_END //( vlr )
