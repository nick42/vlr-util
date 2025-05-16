#include "pch.h"
#include "ModuleContext.LogicalState.h"

namespace vlr {

namespace ModuleContext {

CLogicalState& CLogicalState::GetSharedInstanceMutable()
{
	static auto theInstance = CLogicalState{};
	return theInstance;
}

} // namespace ModuleContext

} // namespace vlr
