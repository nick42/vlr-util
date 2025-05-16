#pragma once

#include <functional>

#include "config.h"

#include "logging.MessageContext.h"
#include "util.Result.h"

namespace vlr {

namespace logging {

class Callbacks
{
public:
	using CheckCouldMessageBeLogged = std::function<SResult(const CMessageContext& oMessageContext)>;
	using LogMessage = std::function<SResult(const CMessageContext& oMessageContext, const vlr::tstring& sMessage)>;

	CheckCouldMessageBeLogged m_fCheckCouldMessageBeLogged;
	LogMessage m_fLogMessage;

public:
	static Callbacks& getSharedInstanceMutable();
	static inline const auto& getSharedInstance()
	{
		return getSharedInstanceMutable();
	}
};

} // namespace logging

} // namespace vlr
