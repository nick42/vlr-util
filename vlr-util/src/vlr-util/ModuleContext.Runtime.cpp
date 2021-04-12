#include "pch.h"
#include "ModuleContext.Runtime.h"

#include <debugapi.h>

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ModuleContext )

NAMESPACE_BEGIN( Runtime )

bool IsDebuggerAttached()
{
	// TODO: Implement this for non-Win32
	auto bAttached = ::IsDebuggerPresent();
	return (!!bAttached);
}

NAMESPACE_END //( Runtime )

NAMESPACE_END //( ModuleContext )

NAMESPACE_END //( vlr )
