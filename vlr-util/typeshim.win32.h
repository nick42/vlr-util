#pragma once

#ifdef _WIN32

#else

#ifdef UNICODE
#define TCHAR wchar_t
#define _T(x) L##x
#else
#define TCHAR char
#define _T(x) x
#endif

// Note: BOOL is defined as int in Windows
// On Apple/Xcode, BOOL is defined in the compiler (as bool)
#ifdef __APPLE__
#include <_types/_uint32_t.h>
#else
using BOOL = int;
#endif
using CHAR = char;
using WCHAR = wchar_t;
using BYTE = unsigned char;
using DWORD = uint32_t;
using DWORD_PTR = unsigned int;
using HRESULT = uint32_t;
using INT = int;
using UINT = unsigned int;
using UINT_PTR = unsigned int;
using INT_PTR = int;

constexpr auto S_OK = HRESULT{ 0U };
constexpr auto S_FALSE = HRESULT{ 1U };
constexpr auto E_FAIL = HRESULT{ 0x80040005U };
constexpr auto E_UNEXPECTED = HRESULT{ 0x8004FFFFU };
constexpr auto E_OUTOFMEMORY = HRESULT{ 0x8007000EL };

#define _HRESULT_DEFINED

#endif

#include "typeshim.win32.ASSERT.h"
