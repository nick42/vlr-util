#pragma once

#include <list>
#include <vector>
#include <optional>
#include <memory>
#include <functional>

#include <gsl/gsl-lite.hpp>

#include <vlr/UtilMacros.Namespace.h>
#include <vlr/util.std_aliases.h>
#include <vlr/zstring_view.h>

NAMESPACE_BEGIN( cpp )

using vlr::string;
using vlr::wstring;
using vlr::tstring;
using vlr::zstring_view;
using vlr::wzstring_view;
using vlr::tzstring_view;

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

using std::function;

using gsl::span;

NAMESPACE_END //( cpp )
