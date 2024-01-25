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

#define CHAR char
#define WCHAR wchar_t
#define DWORD uint32_t
#define HRESULT uint32_t
#define BOOL uint8_t
#define INT int32_t
#define UINT uint32_t

constexpr auto S_OK = HRESULT{0U};
constexpr auto S_FALSE = HRESULT{1U};
constexpr auto E_FAIL = HRESULT{0x80040005};

#define _HRESULT_DEFINED

#endif

#include "typeshim.win32.ASSERT.h"
