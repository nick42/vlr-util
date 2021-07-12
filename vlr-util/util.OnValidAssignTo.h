#pragma once

#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

template< typename TAssignToValue, typename TProspectiveValue, typename std::enable_if_t<std::is_convertible_v<TProspectiveValue, TAssignToValue>>* = nullptr >
constexpr void OnValidAssignTo( TAssignToValue& tValue, const std::optional<TProspectiveValue>& otProspectiveValue )
{
	if (!otProspectiveValue.has_value())
	{
		return;
	}
	tValue = otProspectiveValue.value();
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
