#pragma once

#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#if defined(_WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_AFX))
#define VLR_CONFIG_INCLUDE_ATL_CString 1
#define VLR_CONFIG_INCLUDE_ATL_CComBSTR 1
#else
#define VLR_CONFIG_INCLUDE_ATL_CString 0
#define VLR_CONFIG_INCLUDE_ATL_CComBSTR 0
#endif

#if defined(_WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_BSTR))
#define VLR_CONFIG_INCLUDE_WIN32_bstr_t 1
#else
#define VLR_CONFIG_INCLUDE_WIN32_bstr_t 0
#endif

#if defined(_WIN32)

#include <WinSock2.h>
#include <Windows.h>

#if VLR_CONFIG_INCLUDE_ATL_CString
#include <cstringt.h>
#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
#include <atlcomcli.h>
#endif

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
#include <comdef.h>
#endif

// Other options which can be defined (partial list):

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

// Which elements are included in the logging context
// Note: By default (if config elements not defined), we include all data

#if (!defined(VLR_CONFIG_LOG_CONTEXT_INCLUDE_FILE))
#define VLR_CONFIG_LOG_CONTEXT_INCLUDE_FILE 1
#endif

#if (!defined(VLR_CONFIG_LOG_CONTEXT_INCLUDE_LINE))
#define VLR_CONFIG_LOG_CONTEXT_INCLUDE_LINE 1
#endif

#if (!defined(VLR_CONFIG_LOG_CONTEXT_INCLUDE_FUNCTION))
#define VLR_CONFIG_LOG_CONTEXT_INCLUDE_FUNCTION 1
#endif

// Use this option to have the logging functions allow exceptions within callbacks to be propagated.
// Normally these will be suppressed and swallowed, to allow logging in non-exception contexts.
// #define VLR_CONFIG_LOGGING_ALLOW_EXCEPTIONS

#ifndef _STD
#define _STD std::
#endif

#undef min
#undef max

#endif

#ifdef __has_include

#if __has_include(<boost/program_options.hpp>)
#define VLR_HASDEP_BOOST_PROGRAM_OPTIONS 1
#else
#define VLR_HASDEP_BOOST_PROGRAM_OPTIONS 0
#endif

#if __has_include(<boost/asio/ip/address_v4.hpp>)
#define VLR_HASDEP_BOOST_ASIO 1
#else
#define VLR_HASDEP_BOOST_ASIO 0
#endif

#else

#define VLR_HASDEP_BOOST_PROGRAM_OPTIONS 0
#define VLR_HASDEP_BOOST_ASIO 0

#endif // __has_include

#include "typeshim.win32.h"
#include "util.types.h"
#include "UtilMacros.CompilerSupport.h"
