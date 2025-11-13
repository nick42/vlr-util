#pragma once

#include <list>
#include <vector>
#include <optional>
#include <memory>
#include <functional>

#if __has_include(<span>) && (__cplusplus >= 202002L)
#define VLR_HAS_INCLUDE_STD_SPAN 1
#include <span>
#else
#define VLR_HAS_INCLUDE_STD_SPAN 0
#include <gsl-lite/gsl-lite.hpp>
#endif

#include "util.std_aliases.h"
#include "zstring_view.h"

namespace cpp {

using vlr::string;
using vlr::wstring;
using vlr::tstring;
using vlr::tstring_view;
using vlr::zstring_view;
using vlr::wzstring_view;
using vlr::tzstring_view;

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

using std::function;

#if VLR_HAS_INCLUDE_STD_SPAN
using std::span;
#else
using gsl_lite::span;
#endif

} // namespace cpp
