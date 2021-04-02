#pragma once

#ifdef _WIN32
#include <tchar.h>
#else
#define TCHAR char
#endif

#include "UtilMacros.Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ModuleContext )

NAMESPACE_BEGIN( Compilation )

constexpr auto DefaultCharTypeIs_char()
{
	return (sizeof( TCHAR ) == sizeof( char ));
}

constexpr auto DefaultCharTypeIs_wchar_t()
{
	return (sizeof( TCHAR ) == sizeof( wchar_t ));
}

constexpr auto IsBuildType_Debug()
{
#ifdef _DEBUG
	return true;
#else
	return false;
#endif
}

constexpr auto IsBuildType_Release()
{
	return !IsBuildType_Debug();
}

constexpr auto IsPlatform_Windows()
{
#ifdef WIN32
	return true;
#else
	return false;
#endif
}

NAMESPACE_END //( Compilation )

NAMESPACE_END //( ModuleContext )

NAMESPACE_END //( vlr )
