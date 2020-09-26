#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( security )

NAMESPACE_BEGIN( tokens )

HRESULT SetPrivilegeOnToken(
	HANDLE hToken,
	vlr::tzstring_view svzPrivilegeName,
	bool bEnable );

HRESULT SetPrivilegeOnProcess(
	HANDLE hProcess,
	vlr::tzstring_view svzPrivilegeName,
	bool bEnable );

NAMESPACE_END //( tokens )

NAMESPACE_END //( security )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
