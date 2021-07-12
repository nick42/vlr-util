#pragma once

#include <functional>
#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "BaseWithVirtualDestructor.h"

VLR_NAMESPACE_BEGIN( vlr )

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
	auto DoActionAndClear();

public:
	CActionOnDestruction() = default;
	CActionOnDestruction( const FAction& fAction )
		: m_fAction{ fAction }
	{}
	// Note: We allow move, but not copy, since we do not currently do reference counting
	CActionOnDestruction( CActionOnDestruction&& ) = default;
	CActionOnDestruction( const CActionOnDestruction& ) = delete;
	virtual ~CActionOnDestruction()
	{
		DoActionAndClear();
	}
};

template< typename TFunctor >
inline auto MakeActionOnDestruction( const TFunctor& fAction )
{
	return CActionOnDestruction<decltype(std::declval<TFunctor>()())>{ fAction };
}

template< typename TActionResult >
inline void CActionOnDestruction<TActionResult>::DoAction_VoidResult() const
{
	if (!m_fAction)
	{
		return;
	}

	m_fAction();
}

template< typename TActionResult >
template< typename TMethodResult >
inline TMethodResult CActionOnDestruction<TActionResult>::DoAction_WithResult() const
{
	if (!m_fAction)
	{
		return {};
	}

	return m_fAction();
}

template< typename TActionResult >
inline auto CActionOnDestruction<TActionResult>::DoAction()
{
	if (!m_fAction)
	{
		return std::optional<TActionResult>{};
	}

	return DoAction_WithResult();
}

template<>
inline auto CActionOnDestruction<void>::DoAction()
{
	if (!m_fAction)
	{
		return;
	}

	return DoAction_VoidResult();
}

template< typename TActionResult >
inline auto CActionOnDestruction<TActionResult>::DoActionAndClear()
{
	auto&& tActionResult = DoAction();
	m_fAction = {};
	return tActionResult;
}

template<>
inline auto CActionOnDestruction<void>::DoActionAndClear()
{
	DoAction();
	m_fAction = {};
}

VLR_NAMESPACE_END //( vlr )
