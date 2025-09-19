#pragma once

#include "config.h"

#include <atomic>
#include <functional>

#include "logging.MessageContext.h"
#include "util.DebugBreak.h"
#include "zstring_view.h"

namespace vlr {

namespace assert {

class Callbacks
{
public:
	using FHandleCheckFailure = std::function<void(const vlr::logging::CMessageContext& oMessageContext, vlr::tzstring_view svzFailureMessage)>;

	FHandleCheckFailure m_fHandleCheckFailure;

	std::atomic<bool> m_bOnDefaultHandler_TriggerDebugBreak{ true };

public:
	static Callbacks& getSharedInstanceMutable();
	static const auto& getSharedInstance()
	{
		return getSharedInstanceMutable();
	}

public:
	inline void DefaultHandler(const vlr::logging::CMessageContext& /*oMessageContext*/, vlr::tzstring_view /*svzFailureMessage*/) const
	{
		if (m_bOnDefaultHandler_TriggerDebugBreak)
		{
			util::TriggerDebugBreak();
		}
	}
};

inline void HandleCheckFailure(const vlr::logging::CMessageContext& oMessageContext, vlr::tzstring_view svzFailureMessage)
{
	const auto& fHandleCheckFailure = Callbacks::getSharedInstance().m_fHandleCheckFailure;
	if (fHandleCheckFailure)
	{
		return fHandleCheckFailure(oMessageContext, svzFailureMessage);
	}

	Callbacks::getSharedInstance().DefaultHandler(oMessageContext, svzFailureMessage);
}

} // namespace assert

} // namespace vlr
