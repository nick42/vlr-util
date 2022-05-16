#pragma once

#include <functional>
#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "BaseWithVirtualDestructor.h"

VLR_NAMESPACE_BEGIN(vlr)

template< typename TActionResult >
class CActionOnDestruction
	: public CBaseWithVirtualDestructor
{
	static constexpr bool ActionHasValidResult = !std::is_same_v<TActionResult, void>;

public:
	using FAction = std::function<TActionResult()>;

public:
	FAction m_fAction;

protected:
	decltype(auto) DoActionWithPossibleResult(const FAction& fAction)
	{
		if (!fAction)
		{
			if constexpr (ActionHasValidResult)
			{
				return {};
			}
			else
			{
				return;
			}
		}
		return fAction();
	}

public:
	decltype(auto) DoAction()
	{
		return DoActionWithPossibleResult(m_fAction);
	}
	decltype(auto) DoActionAndClear()
	{
		// Note: Using a "manual" version of this class, to avoid copying m_fAction
		// (we cannot "capture" the result to return, because it might be void)
		auto fClearAction = [this](void*) { m_fAction = {}; };
		// Note: Pointer must not be nullptr, so destructor is called
		auto oOnDelete_ClearAction = std::unique_ptr<void, decltype(fClearAction)>{ (void*)0x42, fClearAction };
		return DoActionWithPossibleResult(m_fAction);
	}

public:
	CActionOnDestruction() = default;
	CActionOnDestruction(const FAction& fAction)
		: m_fAction{ fAction }
	{}
	// Note: We allow move, but not copy, since we do not currently do reference counting
	CActionOnDestruction(CActionOnDestruction&&) = default;
	CActionOnDestruction(const CActionOnDestruction&) = delete;
	virtual ~CActionOnDestruction()
	{
		DoActionAndClear();
	}
};

template< typename TFunctor >
inline auto MakeActionOnDestruction(const TFunctor& fAction)
{
	return CActionOnDestruction<decltype(std::declval<TFunctor>()())>{ fAction };
}

VLR_NAMESPACE_END //(vlr)
