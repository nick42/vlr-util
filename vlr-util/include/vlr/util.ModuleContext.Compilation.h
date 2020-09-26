#pragma once

#include <tchar.h>

#include "UtilMacros.Namespace.h"
#include "config.h"

#ifndef _WIN32
#define TCHAR char
#endif

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

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

NAMESPACE_END //( Compilation )

NAMESPACE_END //( ModuleContext )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
