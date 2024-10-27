#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include <atomic>
#include <functional>

#include "debugbreak.h"
#include "logging.MessageContext.h"
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
	static inline auto& getSharedInstanceMutable()
	{
		static auto theInstance = Callbacks{};
		return theInstance;
	}
	static inline const auto& getSharedInstance()
	{
		return getSharedInstanceMutable();
	}

public:
	inline void DefaultHandler(const vlr::logging::CMessageContext& /*oMessageContext*/, vlr::tzstring_view /*svzFailureMessage*/) const
	{
		if (m_bOnDefaultHandler_TriggerDebugBreak)
		{
			// This uses an open source cross-platform implementation (see header)
			debug_break();
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
