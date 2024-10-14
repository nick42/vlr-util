#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

namespace util {

// Note: This good workaround suggested by MS, for conformance updates related to static_assert
// See: https://docs.microsoft.com/en-us/cpp/overview/cpp-conformance-improvements?view=msvc-170

template <typename...>
struct dependent_false : std::bool_constant<false> { };

// Note: A macro is necessary, because static_assert isn't compatible with parameters
// (it's a "special" function)

#ifdef __APPLE__
// Note: This does not appear to work for the Apple compiler, so removing for now.
#define VLR_TYPE_DEPENDENT_STATIC_FAIL(Type, ...)
#else
#define VLR_TYPE_DEPENDENT_STATIC_FAIL(Type, ...) static_assert(vlr::util::dependent_false<Type>, __VA_ARGS__)
#endif

} // namespace util

} // namespace vlr

