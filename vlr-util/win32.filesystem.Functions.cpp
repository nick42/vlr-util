#include "pch.h"
#include "vlr/win32.filesystem.Functions.h"

#include "vlr/util.data_adaptor.MultiSZ.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( filesystem )

HRESULT GetVolumePathNamesForVolumeName( const vlr::tstring& sVolumeName, std::list<vlr::tstring>& oPathNameList )
{
	TCHAR pszBuffer[2048];
	DWORD dwReturnLength{};
	BOOL bResult = ::GetVolumePathNamesForVolumeName( sVolumeName.c_str(), pszBuffer, 2048, &dwReturnLength );
	if (!bResult)
	{
		return HRESULT_FROM_WIN32( GetLastError() );
	}

	vlr::util::data_adaptor::HelperFor_MultiSZ{}.ToStructuredData( pszBuffer, oPathNameList );

	return S_OK;
}

NAMESPACE_END //( filesystem )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
