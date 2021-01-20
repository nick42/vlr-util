#pragma once

#include <optional>

#include <boost/iterator/iterator_facade.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/ActionOnDestruction.h"
#include "vlr/util.std_aliases.h"
#include "vlr/UtilMacros.Assertions.h"

#include "vlr/win32.registry.iterator_RegEnumValue.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( registry )

class enum_RegValues
{
protected:
	HKEY m_hKey = {};

public:
	inline auto begin() const
	{
		return iterator_RegEnumValue{ m_hKey, 0 };
	}
	inline auto end() const
	{
		return iterator_RegEnumValue{ m_hKey };
	}

public:
	constexpr enum_RegValues( HKEY hKey )
		: m_hKey{ hKey }
	{}
};

NAMESPACE_END //( registry )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
