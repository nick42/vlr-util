#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#if defined(WIN32) && defined(_DLL)
#define _AFXDLL
#endif
