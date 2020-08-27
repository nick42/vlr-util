#pragma once

#include "UtilMacros_Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

template< size_t nIndex >
struct choice : public choice<nIndex + 1> {};

// An arbitrary cutoff...
template<>
struct choice<10> {};

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
