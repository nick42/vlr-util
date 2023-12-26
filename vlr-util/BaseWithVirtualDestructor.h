#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

class CBaseWithVirtualDestructor
{
public:
	virtual ~CBaseWithVirtualDestructor() = default;
};

} // namespace vlr
