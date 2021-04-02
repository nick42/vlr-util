#include "pch.h"
#include "vlr/ops.NetworkTargetInfo.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ops )

cpp::tzstring_view CNetworkTargetInfo::GetNameForIntent_win32_OpenSCManager() const
{
	if (m_bIsLocalSystem)
	{
		// NULL is correct for local system
		return cpp::tzstring_view{};
	}

	// TODO: Write method to switch on available info, check valid

	return m_sTargetName_Logical;
}

NAMESPACE_END //( ops )

NAMESPACE_END //( vlr )
