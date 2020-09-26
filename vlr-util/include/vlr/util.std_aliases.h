#pragma once

#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

// TODO? Replace this with non-preprocessor impl
#ifndef UNICODE
using tstring = std::string;
#else
using tstring = std::wstring;
#endif

NAMESPACE_END //( vlr )
