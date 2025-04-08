#pragma once

#include <string>

#include "config.h"

namespace vlr {

// TODO? Replace this with non-preprocessor impl
#ifndef UNICODE
using tstring = std::string;
#else
using tstring = std::wstring;
#endif

using string = std::string;
using wstring = std::wstring;

} // namespace vlr
