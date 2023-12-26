#include "pch.h"
#include "ops.NetworkTargetInfo.h"

namespace vlr {

namespace ops {

vlr::tzstring_view CNetworkTargetInfo::GetNameForIntent_win32_OpenSCManager() const
{
	if (m_bIsLocalSystem)
	{
		// NULL is correct for local system
		return vlr::tzstring_view{};
	}

	// TODO: Write method to switch on available info, check valid

	return m_sTargetName_Logical;
}

} // namespace ops

} // namespace vlr
