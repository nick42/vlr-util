#include "pch.h"
#include "ModuleContext.Runtime.h"

#include <debugapi.h>

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( ModuleContext )

VLR_NAMESPACE_BEGIN( Runtime )

bool IsDebuggerAttached()
{
	// TODO: Implement this for non-Win32
	auto bAttached = ::IsDebuggerPresent();
	return (!!bAttached);
}

VLR_NAMESPACE_END //( Runtime )

VLR_NAMESPACE_END //( ModuleContext )

VLR_NAMESPACE_END //( vlr )
