#pragma once

#ifdef _WIN32
#include <tchar.h>
#else
#define TCHAR char
#endif

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

namespace ModuleContext {

namespace Compilation {

constexpr auto IsBuildPlatform_Win32()
{
#ifdef WIN32
	return true;
#else
	return false;
#endif
}

constexpr auto IsBuildPlatform_Linux()
{
#ifdef __linux__
	return true;
#else
	return false;
#endif
}

constexpr auto IsBuildPlatform_Apple()
{
#ifdef __APPLE__
	return true;
#else
	return false;
#endif
}

constexpr auto IsSymbolDefined_DEBUG()
{
#ifdef DEBUG
	return true;
#else
	return false;
#endif
}

constexpr auto IsSymbolDefined__DEBUG()
{
#ifdef _DEBUG
	return true;
#else
	return false;
#endif
}

constexpr auto IsSymbolDefined_NDEBUG()
{
#ifdef NDEBUG
	return true;
#else
	return false;
#endif
}

constexpr auto DefaultCharTypeIs_char()
{
	return (sizeof(TCHAR) == sizeof(char));
}

constexpr auto DefaultCharTypeIs_wchar_t()
{
	return (sizeof(TCHAR) == sizeof(wchar_t));
}

constexpr auto IsBuildType_Debug()
{
	if constexpr (IsBuildPlatform_Win32())
	{
		return (IsSymbolDefined__DEBUG() || !IsSymbolDefined_NDEBUG());
	}
	else
	{
		// No standard for other platforms? C-runtime uses NDEBUG to gate assert()...
		return !IsSymbolDefined_NDEBUG();
	}
}

constexpr auto IsBuildType_Release()
{
	return !IsBuildType_Debug();
}

} // namespace Compilation

} // namespace ModuleContext

} // namespace vlr
