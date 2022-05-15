#pragma once

#ifndef _WIN32
#else

#include <ATLComTime.h>

using DateTime = ATL::COleDateTime;
using DateTimeSpan = ATL::COleDateTimeSpan;

#endif
