#pragma once

//#if defined(_WIN32) && VLR_CONFIG_INCLUDE_ATL_CString
#if defined(ASSERT)
#else

#include <assert.h>

#define ASSERT assert

#endif
