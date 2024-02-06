#pragma once

#include <functional>

#include "util.Result.h"

namespace vlr {

template <class _Fty>
class CDynamicLoadedFunction
{
public:
	SResult m_srLoadResult;
	std::function<_Fty> m_fFunction;
};

} // namespace vlr
