#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

namespace util {

// Note: This good workaround suggested by MS, for conformance updates related to static_assert
// See: https://docs.microsoft.com/en-us/cpp/overview/cpp-conformance-improvements?view=msvc-170

template<bool bValue = false>
constexpr bool dependent_false = bValue;

// Note: The above does not work for GCC, because it cannot deal with a template type without 
// specifying the template parameter. Unfortunately, MSVC cannot do the latter. =/
#ifdef _MSC_VER
#define VLR_DEPENDENT_FALSE vlr::util::dependent_false
#endif
#ifdef __GNUC__
#define VLR_DEPENDENT_FALSE vlr::util::dependent_false<false>
#endif

//template< typename TString, bool flag = false >
//constexpr void static_assert_fail(TString&& tString)
//{
//	static_assert(flag, tString);
//}

// Note: A macro is necessary, because static_assert isn't compatible with parameters
// (it's a "special" function)

#define VLR_STATIC_FAIL(...) static_assert(VLR_DEPENDENT_FALSE, __VA_ARGS__)

} // namespace util

} // namespace vlr

