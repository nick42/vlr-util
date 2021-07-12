#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

VLR_NAMESPACE_BEGIN( vlr )

class CBaseWithVirtualDestructor
{
public:
	virtual ~CBaseWithVirtualDestructor() = default;
};

VLR_NAMESPACE_END //( vlr )
