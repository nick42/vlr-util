#pragma once

#include <optional>

#include "config.h"

namespace vlr {

namespace util {

template< typename TAssignToValue, typename TProspectiveValue, typename std::enable_if_t<std::is_convertible_v<TProspectiveValue, TAssignToValue>>* = nullptr >
constexpr void OnValidAssignTo( TAssignToValue& tValue, const std::optional<TProspectiveValue>& otProspectiveValue )
{
	if (!otProspectiveValue.has_value())
	{
		return;
	}
	tValue = otProspectiveValue.value();
}

} // namespace util

} // namespace vlr
