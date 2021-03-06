#pragma once

#include "UtilMacros.Namespace.h"
#include "cpp_namespace.h"
#include "util.Result.h"

#include "ops.NetworkTargetInfo.h"
#include "util.AutoCleanup_SC_HANDLE.h"
#include "win32.ServiceConfig.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

class CServiceControl
{
protected:
	cpp::shared_ptr<util::CAutoCleanup_SC_HANDLE> m_spAutoCleanupSCM;

	std::optional<SC_HANDLE> GetOpenHandle_SCM() const;

public:
	util::CResult Connect(
		const ops::CNetworkTargetInfo& oNetworkTargetInfo,
		DWORD dwDesiredAccess = SC_MANAGER_ALL_ACCESS );

	util::CResult SCM_CreateService(
		const CServiceConfig& oServiceConfig,
		SC_HANDLE& hService_Result );

	util::CResult SCM_DeleteService(
		SC_HANDLE hService );
	util::CResult SCM_DeleteService(
		const CServiceConfig& oServiceConfig );

	util::CResult SCM_OpenService(
		vlr::tzstring_view svzServiceName,
		DWORD dwDesiredAccess,
		SC_HANDLE& hService_Result );

};

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
