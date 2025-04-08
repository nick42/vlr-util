#pragma once

#include "config.h"

#include "util.static_assert.h"

namespace vlr {

namespace c_runtime {

namespace StringFunctions {

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
		VLR_TYPE_DEPENDENT_STATIC_FAIL(TChar, "Unhandled TChar type");
	}
}

} // namespace StringFunctions

} // namespace c_runtime

} // namespace vlr
