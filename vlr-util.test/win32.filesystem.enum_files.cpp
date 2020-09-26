#include "pch.h"

#include "vlr/win32.filesystem.enum_files.h"

TEST( win32_filesystem_enum_files, general )
{
	auto oEnumFiles = vlr::win32::filesystem::enum_files{};
	oEnumFiles.m_sSearchString = _T( "C:\\*" );

	for (const auto& pFileData : oEnumFiles)
	{
		TRACE( _T( "File name: %s\n" ), pFileData->cFileName );
	}
}
