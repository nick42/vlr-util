#pragma once

#ifdef WIN32

#include <winsdkver.h>

#define WINVER _WIN32_WINNT_WIN7
#define _WIN32_WINNT WINVER

#include <sdkddkver.h>

#endif
