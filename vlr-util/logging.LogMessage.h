#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "include.spdlog.h"

#include "logging.MessageContext.h"
#include "formatpf.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( logging )

inline auto GetLevel_spdlog( const CMessageContext& oMessageContext )
{
	switch (oMessageContext.m_eLogicalLevel)
	{
	case LogicalLevel::Debug:
		return spdlog::level::debug;
	case LogicalLevel::Trace:
	case LogicalLevel::Verbose:
		return spdlog::level::trace;
	default:
	case LogicalLevel::Info:
		return spdlog::level::info;
	case LogicalLevel::Warning:
		return spdlog::level::warn;
	case LogicalLevel::Error:
		return spdlog::level::err;
	case LogicalLevel::Critical:
		return spdlog::level::critical;
	}
}

template< typename TString >
inline auto LogMessage( const CMessageContext& oMessageContext, const TString& tMessage )
{
	// TODO? Pre-checking based on message context, if message will be logged, early abort (before formatting)

	auto eLevel_spdlog = GetLevel_spdlog( oMessageContext );
	spdlog::log( eLevel_spdlog, tMessage );

	return tMessage;
}

template< typename TFormatString, typename... Arg >
inline auto LogMessagePF( const CMessageContext& oMessageContext, TFormatString svFormatString, Arg&&... args )
{
	// TODO? Pre-checking based on message context, if message will be logged, early abort (before formatting)

	auto sMessage = formatpf( svFormatString, std::forward<Arg>( args )... );

	auto eLevel_spdlog = GetLevel_spdlog( oMessageContext );
	spdlog::log( eLevel_spdlog, sMessage );

	return sMessage;
}

VLR_NAMESPACE_END //( logging )

VLR_NAMESPACE_END //( vlr )
