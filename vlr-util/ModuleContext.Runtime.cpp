#include "pch.h"
#include "ModuleContext.Runtime.h"

#include <vlr-util/ModuleContext.Compilation.h>

#if defined(_WIN32)
#include <debugapi.h>
#endif

namespace vlr {

namespace ModuleContext {

namespace Runtime {

bool IsDebuggerAttached()
{
#if defined(_WIN32)
	auto bAttached = ::IsDebuggerPresent();
	return (!!bAttached);
#else
	// TODO: Implement this for non-Win32
	return false;
#endif
}

} // namespace Runtime

} // namespace ModuleContext

} // namespace vlr
