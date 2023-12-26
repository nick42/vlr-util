#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#if defined(_WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_AFX))
#define VLR_CONFIG_INCLUDE_ATL_CSTRING 1
#else
#define VLR_CONFIG_INCLUDE_ATL_CSTRING 0
#endif

#if defined(_WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_BSTR))
#define VLR_CONFIG_INCLUDE_WIN32_BSTR 1
#else
#define VLR_CONFIG_INCLUDE_WIN32_BSTR 0
#endif

#if defined(_WIN32)

#include <WinSock2.h>
#include <Windows.h>

#if VLR_CONFIG_INCLUDE_ATL_CSTRING
#include <cstringt.h>
#endif

#if VLR_CONFIG_INCLUDE_WIN32_BSTR
#include <comdef.h>
#endif

#ifndef _STD
#define _STD std::
#endif

#undef min
#undef max

#endif

#include "typeshim.win32.h"
#include "util.types.h"
