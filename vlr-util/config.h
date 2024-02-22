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

// Other options which can be defined (partial list:

// VLR_FALLBACK_Inline_UTF16_to_MultiByte_StdString
// Prototype: std::string function(std::wstring_view svValue, const StringConversionOptions & oConversionOptions);
// Used by: string conversion code, if native conversion is not available

// VLR_FALLBACK_Inline_MultiByte_to_UTF16_StdString
// Prototype: std::wstring function(std::string_view svValue, const StringConversionOptions & oConversionOptions);
// Used by: string conversion code, if native conversion is not available

// VLR_CONFIG_ASSERTIONS_INCLUDE_FUNCTION
// Define this to have assertion failure messages include the function name as a context prefix

// VLR_CONFIG_ASSERTIONS_FAILURE_MESSAGE_PREFIX
// Define this to specify a custom assertion failure message prefix (as _T("...") string)

#ifndef _STD
#define _STD std::
#endif

#undef min
#undef max

#endif

#include "typeshim.win32.h"
#include "util.types.h"
