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
using BOOL = int;
using CHAR = char;
using WCHAR = wchar_t;
using DWORD = uint32_t;
using DWORD_PTR = unsigned int;
using HRESULT = uint32_t;
using INT = int;
using UINT = unsigned int;

constexpr auto S_OK = HRESULT{0U};
constexpr auto S_FALSE = HRESULT{1U};
constexpr auto E_FAIL = HRESULT{0x80040005};

#define _HRESULT_DEFINED

#endif

#include "typeshim.win32.ASSERT.h"
