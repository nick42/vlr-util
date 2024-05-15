#pragma once

#include <functional>

#include "util.Result.h"

namespace vlr {

class CDynamicLoadedFunctionBase
{
public:
	SResult m_srLoadResult;
	void* m_pvRawFunctionPointer = {};
};
using SPCDynamicLoadedFunctionBase = std::shared_ptr<CDynamicLoadedFunctionBase>;

template <class _Fty>
class CDynamicLoadedFunction
	: public CDynamicLoadedFunctionBase
{
public:
	using FunctionPrototype = _Fty;

	std::function<_Fty> m_fFunction;

public:
	decltype(auto) GetFunction() const
	{
		return m_fFunction;
	}

public:
	SResult SetFromRawPtr(void* pvRawFunctionPointer)
	{
		m_pvRawFunctionPointer = pvRawFunctionPointer;
		// Note: This requires an effective C-style cast, and is generally "unsafe"
		m_fFunction = (FunctionPrototype*)pvRawFunctionPointer;

		return SResult::Success;
	}
};

} // namespace vlr
