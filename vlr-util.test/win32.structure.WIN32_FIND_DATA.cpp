#include "pch.h"

#include "vlr-util/win32.structure.WIN32_FIND_DATA.h"

TEST( win32_structure_WIN32_FIND_DATA, general )
{
	EXPECT_EQ( sizeof( vlr::win32::structure::CAccessorFor_WIN32_FIND_DATA ), sizeof( WIN32_FIND_DATA ) );

	{
		auto oFindInfo = vlr::win32::structure::CAccessorFor_WIN32_FIND_DATA{};
		oFindInfo.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		EXPECT_EQ( oFindInfo.HasAttribute_Directory(), true );
	}
}
