#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( c_runtime )

NAMESPACE_BEGIN( StringFunctions )

template< typename TChar = TCHAR >
inline auto GetFunction_strlen()
{
	if constexpr (std::is_same_v<TChar, char>)
	{
		return &strlen;
	}
	else if constexpr (std::is_same_v<TChar, wchar_t>)
	{
		return &wcslen;
	}
	else
	{
		static_assert(false, "Unhandled TChar type");
	}
}

NAMESPACE_END //( StringFunctions )

NAMESPACE_END //( c_runtime )

NAMESPACE_END //( vlr )
