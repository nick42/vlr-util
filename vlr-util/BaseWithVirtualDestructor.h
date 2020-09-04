#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

class CBaseWithVirtualDestructor
{
public:
	virtual ~CBaseWithVirtualDestructor() = default;
};

NAMESPACE_END //( vlr )
