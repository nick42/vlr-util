#include "pch.h"
#include "vlr/win32.security.tokens.h"

#include "vlr/AutoFreeResource.h"
#include "vlr/UtilMacros.Assertions.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( security )

NAMESPACE_BEGIN( tokens )

HRESULT SetPrivilegeOnToken(
	HANDLE hToken,
    vlr::tzstring_view svzPrivilegeName,
    bool bEnable )
{
    BOOL bResult{};

    LUID nPrivilegeID{};
    bResult = ::LookupPrivilegeValue(
        NULL,
        svzPrivilegeName,
        &nPrivilegeID );
    if (!bResult)
    {
        auto dwLastError = ::GetLastError();
        return HRESULT_FROM_WIN32( dwLastError );
    }

    TOKEN_PRIVILEGES oTokenPrivileges{};
    oTokenPrivileges.PrivilegeCount = 1;
    auto& oPrivilege0 = oTokenPrivileges.Privileges[0];
    oPrivilege0.Luid = nPrivilegeID;
    oPrivilege0.Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

    bResult = ::AdjustTokenPrivileges(
        hToken,
        FALSE,
        &oTokenPrivileges,
        sizeof( TOKEN_PRIVILEGES ),
        nullptr,
        nullptr );
    if (!bResult)
    {
        auto dwLastError = ::GetLastError();
        // Note: ERROR_NOT_ALL_ASSIGNED if we could not set privilege on token...
        return HRESULT_FROM_WIN32( dwLastError );
    }

    return S_OK;
}

HRESULT SetPrivilegeOnProcess(
    HANDLE hProcess,
    vlr::tzstring_view svzPrivilegeName,
    bool bEnable )
{
    BOOL bResult{};

    HANDLE hToken{};
    bResult = ::OpenProcessToken(
        hProcess,
        TOKEN_ADJUST_PRIVILEGES,
        &hToken );
    ASSERT_NONZERO__OR_RETURN_HRESULT_LAST_ERROR( bResult );
    auto oOnDestroy_FreeToken = vlr::MakeAutoCleanup_viaCloseHandle( hToken );

    return SetPrivilegeOnToken( hToken, svzPrivilegeName, bEnable );
}

NAMESPACE_END //( tokens )

NAMESPACE_END //( security )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
