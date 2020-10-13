#pragma once

#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

template< typename TAssignToValue, typename TProspectiveValue, typename std::enable_if_t<std::is_convertible_v<TProspectiveValue, TAssignToValue>>* = nullptr >
constexpr void OnValidAssignTo( TAssignToValue& tValue, const std::optional<TProspectiveValue>& otProspectiveValue )
{
	if (!otProspectiveValue.has_value())
	{
		return;
	}
	tValue = otProspectiveValue.value();
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
