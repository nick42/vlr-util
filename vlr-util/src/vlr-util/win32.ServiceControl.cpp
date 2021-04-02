#include "pch.h"
#include "Win32.ServiceControl.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

std::optional<SC_HANDLE> CServiceControl::GetOpenHandle_SCM() const
{
	static constexpr auto _tFailureValue = std::optional<SC_HANDLE>{};

	if (!m_spAutoCleanupSCM)
	{
		return {};
	}
	if (m_spAutoCleanupSCM->m_hSCM == nullptr)
	{
		HANDLE_ASSERTION_FAILURE__AND_RETURN_FAILURE_VALUE;
	}

	return m_spAutoCleanupSCM->m_hSCM;
}

util::CResult CServiceControl::Connect(
	const ops::CNetworkTargetInfo& oNetworkTargetInfo,
	DWORD dwDesiredAccess /*= SC_MANAGER_ALL_ACCESS*/ )
{
	m_spAutoCleanupSCM = {};

	auto hSCM = ::OpenSCManager(
		oNetworkTargetInfo.GetNameForIntent_win32_OpenSCManager(),
		SERVICES_ACTIVE_DATABASE,
		dwDesiredAccess );
	if (hSCM == NULL)
	{
		return util::CResult::For_win32_LastError();
	}

	m_spAutoCleanupSCM = cpp::make_shared<util::CAutoCleanup_SC_HANDLE>( hSCM );

	return S_OK;
}

util::CResult CServiceControl::SCM_CreateService(
	const CServiceConfig& oServiceConfig,
	SC_HANDLE& hService_Result )
{
	auto ohSCM = GetOpenHandle_SCM();
	if (!ohSCM.has_value())
	{
		return E_UNEXPECTED;
	}

	// Service name must be valid
	ASSERT_NOTBLANK__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName );
	ASSERT_NOTBLANK__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName_Display );

	// Some additional checks for validity, per docs
	ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName.size(), <= , 256 );
	ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName.find( _T( '/' ) ), == , oServiceConfig.m_svzServiceName.npos );
	ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName.find( _T( '\\' ) ), == , oServiceConfig.m_svzServiceName.npos );
	ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName_Display.size(), <= , 256 );

	auto hService = ::CreateService(
		ohSCM.value(),
		oServiceConfig.m_svzServiceName,
		oServiceConfig.m_svzServiceName_Display,
		oServiceConfig.m_dwDesiredAccess,
		oServiceConfig.m_dwServiceType,
		oServiceConfig.m_dwStartType,
		oServiceConfig.m_dwErrorControl,
		oServiceConfig.m_sFilePath_ServiceBinary.c_str(),
		oServiceConfig.m_svzLoadOrderGroup,
		oServiceConfig.m_lpdwTagId,
		oServiceConfig.m_svzDependencies,
		oServiceConfig.m_svzRunAsAccount_Username,
		oServiceConfig.m_svzRunAsAccount_Password );
	if (hService == NULL)
	{
		return util::CResult::For_win32_LastError();
	}

	hService_Result = hService;

	return S_OK;
}

util::CResult CServiceControl::SCM_DeleteService(
	SC_HANDLE hService )
{
	auto bResult = ::DeleteService(
		hService );
	if (!bResult)
	{
		return util::CResult::For_win32_LastError();
	}

	return S_OK;
}

util::CResult CServiceControl::SCM_DeleteService(
	const CServiceConfig& oServiceConfig )
{
	HRESULT hr;

	// Service name must be valid
	ASSERT_NOTBLANK__OR_RETURN_EUNEXPECTED( oServiceConfig.m_svzServiceName );

	SC_HANDLE hService{};
	hr = SCM_OpenService(
		oServiceConfig.m_svzServiceName,
		DELETE,
		hService );
	ON_HR_NON_S_OK__RETURN_HRESULT( hr );
	auto oOnDestroy_CloseService = util::CAutoCleanup_SC_HANDLE{ hService };

	return SCM_DeleteService( hService );
}

util::CResult CServiceControl::SCM_OpenService(
	vlr::tzstring_view svzServiceName,
	DWORD dwDesiredAccess,
	SC_HANDLE& hService_Result )
{
	auto ohSCM = GetOpenHandle_SCM();
	if (!ohSCM.has_value())
	{
		return E_UNEXPECTED;
	}

	auto hService = ::OpenService(
		ohSCM.value(),
		svzServiceName,
		dwDesiredAccess );
	if (hService == NULL)
	{
		return util::CResult::For_win32_LastError();
	}

	hService_Result = hService;

	return S_OK;
}

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
