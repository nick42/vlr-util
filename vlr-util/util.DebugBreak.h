#pragma once

#include "config.h"
#include "debugbreak.h"
#include "ModuleContext.Compilation.h"

namespace vlr {

namespace util {

// Note: This will only trigger a debug-break in a debug build
// Note: This uses an open source cross-platform implementation (see header)
// Note: Can potentially add other conditions here, if desired
inline void TriggerDebugBreak()
{
	if constexpr (ModuleContext::Compilation::IsBuildType_Debug())
	{
		debug_break();
	}
}

} // namespace util

} // namespace vlr
