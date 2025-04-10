#pragma once

#include "config.h"

#include "logging.Callbacks.h"
#include "logging.MessageContext.h"
#include "formatpf.h"
#include "util.convert.StringConversion.h"

#if VLR_HASDEP_SPDLOG
#include "include.spdlog.h"
#endif // VLR_HASDEP_SPDLOG

namespace vlr {

namespace logging {

namespace DefaultImpl {

#if VLR_HASDEP_SPDLOG

inline auto GetLevel_spdlog(const CMessageContext& oMessageContext)
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

#endif // VLR_HASDEP_SPDLOG

inline SResult CheckCouldMessageBeLogged(const CMessageContext& /*oMessageContext*/)
{
	return SResult::Success;
}

inline SResult LogMessage([[maybe_unused]] const CMessageContext& oMessageContext, [[maybe_unused]] const vlr::tstring& sMessage)
{
#if VLR_HASDEP_SPDLOG
	auto eLevel_spdlog = GetLevel_spdlog(oMessageContext);
	spdlog::log(eLevel_spdlog, sMessage);

	return SResult::Success;
#else
	return SResult::Success_NoWorkDone;
#endif
}

} // namespace DefaultImpl

} // namespace logging

} // namespace vlr
