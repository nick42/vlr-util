#include "pch.h"

#include "vlr/win32.filesystem.enum_volumes.h"
#include "vlr/util.data_adaptor.MultiSZ.h"

TEST( win32_filesystem_enum_volumes, general )
{
	for (const auto& sVolumeName : vlr::win32::filesystem::enum_volumes{})
	{
		TRACE( _T( "Volume name: %s\n" ), sVolumeName.c_str() );

		TCHAR pszBuffer[2048];
		DWORD dwReturnLength{};
		GetVolumePathNamesForVolumeName( sVolumeName.c_str(), pszBuffer, 2048, &dwReturnLength );

		std::list<vlr::tstring> oValueList;
		vlr::util::data_adaptor::HelperFor_MultiSZ{}.ToStructuredData( pszBuffer, oValueList );
	}
}
