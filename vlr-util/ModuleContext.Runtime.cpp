#include "pch.h"
#include "ModuleContext.Runtime.h"

#include <vlr-util/ModuleContext.Compilation.h>

#if defined(_WIN32)
#include <debugapi.h>
#endif

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( ModuleContext )

VLR_NAMESPACE_BEGIN( Runtime )

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

VLR_NAMESPACE_END //( Runtime )

VLR_NAMESPACE_END //( ModuleContext )

VLR_NAMESPACE_END //( vlr )
