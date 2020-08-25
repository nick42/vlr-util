#pragma once

#include <functional>
#include <optional>

#include "UtilMacros_Namespace.h"
#include "config.h"

#include "BaseWithVirtualDestructor.h"

NAMESPACE_BEGIN( vlr )

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
	void DoAction_VoidResult() const;
	template< typename TMethodResult = std::enable_if_t<ActionHasValidResult, std::optional<TActionResult>> >
	TMethodResult DoAction_WithResult() const;

public:
	auto DoAction();

public:
	CActionOnDestruction() = default;
	CActionOnDestruction( const FAction& fAction )
		: m_fAction{ fAction }
	{}
	virtual ~CActionOnDestruction()
	{
		DoAction();
	}
};

template< typename TFunctor >
inline auto MakeActionOnDestruction( const TFunctor& fAction )
{
	return CActionOnDestruction<decltype(std::declval<TFunctor>()())>{ fAction };
}

template< typename TActionResult >
void CActionOnDestruction<TActionResult>::DoAction_VoidResult() const
{
	if (!m_fAction)
	{
		return;
	}

	m_fAction();
}

template< typename TActionResult >
template< typename TMethodResult >
TMethodResult CActionOnDestruction<TActionResult>::DoAction_WithResult() const
{
	if (!m_fAction)
	{
		return {};
	}

	return m_fAction();
}

template< typename TActionResult >
auto CActionOnDestruction<TActionResult>::DoAction()
{
	if (!m_fAction)
	{
		if constexpr (ActionHasValidResult)
		{
			return std::optional<TActionResult>{};
		}
		else
		{
			return;
		}
	}

	if constexpr (ActionHasValidResult)
	{
		return DoAction_WithResult();
	}
	else
	{
		return DoAction_VoidResult();
	}
}

NAMESPACE_END //( vlr )
