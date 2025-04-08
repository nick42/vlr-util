#pragma once

#include "config.h"

namespace vlr {

namespace util {

template< size_t nIndex >
struct choice : public choice<nIndex + 1> {};

// An arbitrary cutoff...
template<>
struct choice<10> {};

} // namespace util

} // namespace vlr
