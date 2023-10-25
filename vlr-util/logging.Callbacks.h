#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "logging.MessageContext.h"
#include "util.Result.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(logging)

class Callbacks
{
public:
	using CheckCouldMessageBeLogged = std::function<SResult(const CMessageContext& oMessageContext)>;
	using LogMessage = std::function<SResult(const CMessageContext& oMessageContext, const vlr::tstring& sMessage)>;

	CheckCouldMessageBeLogged m_fCheckCouldMessageBeLogged;
	LogMessage m_fLogMessage;

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
};

VLR_NAMESPACE_END //(logging)

VLR_NAMESPACE_END //(vlr)
