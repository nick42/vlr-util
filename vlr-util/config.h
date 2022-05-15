#pragma once

#if defined(_WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_AFX))
#define VLR_CONFIG_INCLUDE_AFX 1
#else
#define VLR_CONFIG_INCLUDE_AFX 0
#endif

#ifdef _DLL
#define _AFXDLL
#endif

#if VLR_CONFIG_INCLUDE_AFX
#include <afx.h>
#endif

#ifndef _STD
#define _STD std::
#endif

#include "typeshim.win32.h"
#include "util.types.h"
