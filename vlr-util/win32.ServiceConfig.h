#pragma once

#include "UtilMacros.Namespace.h"
#include "cpp_namespace.h"

#include "util.logical_zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

class CServiceConfig
{
public:
	util::logical_tzstring_view m_svzServiceName;
	util::logical_tzstring_view m_svzServiceName_Display;
	DWORD m_dwDesiredAccess = SERVICE_ALL_ACCESS;
	DWORD m_dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	DWORD m_dwStartType = SERVICE_DEMAND_START;
	DWORD m_dwErrorControl = SERVICE_ERROR_NORMAL;
	vlr::tstring m_sFilePath_ServiceBinary;
	util::logical_tzstring_view m_svzLoadOrderGroup;
	LPDWORD m_lpdwTagId = nullptr;
	util::logical_tzstring_view m_svzDependencies;
	util::logical_tzstring_view m_svzRunAsAccount_Username;
	util::logical_tzstring_view m_svzRunAsAccount_Password;

public:
	decltype(auto) With_ServiceName( vlr::tzstring_view svzServiceName )
	{
		m_svzServiceName = svzServiceName;
		return *this;
	}
	decltype(auto) With_ServiceName_Display( vlr::tzstring_view svzServiceName_Display )
	{
		m_svzServiceName_Display = svzServiceName_Display;
		return *this;
	}
	decltype(auto) With_FilePath_ServiceBinary( vlr::tstring sFilePath_ServiceBinary )
	{
		m_sFilePath_ServiceBinary = sFilePath_ServiceBinary;
		return *this;
	}

};

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
