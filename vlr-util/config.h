#pragma once

#if defined(_WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_AFX))
#define VLR_CONFIG_INCLUDE_AFX 1
#endif

#ifdef _DLL
#define _AFXDLL
#endif

#if VLR_CONFIG_INCLUDE_AFX
#include <afx.h>
#endif

#ifndef _WIN32
#ifdef UNICODE
#define TCHAR wchar_t
#else
#define TCHAR char
#endif
#endif

#ifndef _STD
#define _STD std::
#endif
