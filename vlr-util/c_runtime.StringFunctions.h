#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( c_runtime )

VLR_NAMESPACE_BEGIN( StringFunctions )

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
		static_assert(VLR_DEPENDENT_FALSE, "Unhandled TChar type");
	}
}

VLR_NAMESPACE_END //( StringFunctions )

VLR_NAMESPACE_END //( c_runtime )

VLR_NAMESPACE_END //( vlr )
