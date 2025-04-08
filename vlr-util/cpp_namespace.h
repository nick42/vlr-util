#pragma once

#include <list>
#include <vector>
#include <optional>
#include <memory>
#include <functional>

#include <gsl/gsl-lite.hpp>

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

using gsl::span;

} // namespace cpp
