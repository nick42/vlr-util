#pragma once

#include <list>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/util.std_aliases.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( filesystem )

HRESULT GetVolumePathNamesForVolumeName( const vlr::tstring& sVolumeName, std::list<vlr::tstring>& oPathNameList );

NAMESPACE_END //( filesystem )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
