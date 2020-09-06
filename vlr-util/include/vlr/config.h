#pragma once

#if defined(WIN32) && (!defined(VLR_CONFIG_EXCLUDE_WIN32_AFX))
#define VLR_CONFIG_INCLUDE_AFX 1
#endif

#ifdef _DLL
#define _AFXDLL
#endif

#if VLR_CONFIG_INCLUDE_AFX
#include <afx.h>
#endif
