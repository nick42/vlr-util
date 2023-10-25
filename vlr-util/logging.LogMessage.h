#pragma once

#include <atomic>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "logging.MessageContext.h"
#include "util.choice.h"
#include "util.convert.StringConversion.h"

#ifndef VLR_CONSUMER_IMPL_LOGGING_CALLBACKS
#include "logging.LogMessage.DefaultImpl.h"
#else
#include "logging.Callbacks.h"
#endif

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(logging)

inline void BootstrapCallbacksOnce()
{
	static std::atomic<bool> atomicRunThisTime = false;
	if (atomicRunThisTime.exchange(true))
	{
		// Already ran
		return;
	}
#ifndef VLR_CONSUMER_IMPL_LOGGING_CALLBACKS
	Callbacks::getSharedInstanceMutable().m_fCheckCouldMessageBeLogged = &DefaultImpl::CheckCouldMessageBeLogged;
	Callbacks::getSharedInstanceMutable().m_fLogMessage = &DefaultImpl::LogMessage;
#endif
}

template <typename T>
struct TFormatResult
{
	using type =
		std::conditional_t<std::is_same<T, std::string>::value, std::string,
		std::conditional_t<std::is_same<T, std::wstring>::value, std::wstring,
		std::conditional_t<std::is_convertible<T, const char*>::value, std::string,
		std::conditional_t<std::is_convertible<T, const wchar_t*>::value, std::wstring,
		void>>>>;  // Default to void if no conversion found
};

template< typename TString >
inline auto LogMessage(const CMessageContext& oMessageContext, const TString& tMessage)
-> typename TFormatResult<TString>::type
{
	BootstrapCallbacksOnce();

	SResult sr;
	const auto& oCallbacks = Callbacks::getSharedInstance();

	sr = oCallbacks.m_fCheckCouldMessageBeLogged(oMessageContext);
	if (sr != SResult::Success)
	{
		return {};
	}

	/*sr =*/ oCallbacks.m_fLogMessage(oMessageContext, util::Convert::ToStdString(tMessage));

	return tMessage;
}

template< typename TFormatString, typename... Arg >
inline auto LogMessagePF(const CMessageContext& oMessageContext, TFormatString svFormatString, Arg&&... args)
-> typename TFormatResult<TFormatString>::type
{
	BootstrapCallbacksOnce();

	SResult sr;
	const auto& oCallbacks = Callbacks::getSharedInstance();

	sr = oCallbacks.m_fCheckCouldMessageBeLogged(oMessageContext);
	if (sr != SResult::Success)
	{
		return {};
	}

	auto sMessage = formatpf(svFormatString, std::forward<Arg>(args)...);

	/*sr =*/ oCallbacks.m_fLogMessage(oMessageContext, util::Convert::ToStdString(sMessage));

	return sMessage;
}

template< typename TFormatString, typename... Arg >
inline auto LogMessageFmt(const CMessageContext& oMessageContext, TFormatString svFormatString, Arg&&... args)
-> typename TFormatResult<TFormatString>::type
{
	BootstrapCallbacksOnce();

	SResult sr;
	const auto& oCallbacks = Callbacks::getSharedInstance();

	sr = oCallbacks.m_fCheckCouldMessageBeLogged(oMessageContext);
	if (sr != SResult::Success)
	{
		return {};
	}

	auto sMessage = fmt::format(svFormatString, std::forward<Arg>(args)...);

	/*sr =*/ oCallbacks.m_fLogMessage(oMessageContext, util::Convert::ToStdString(sMessage));

	return sMessage;
}

VLR_NAMESPACE_END //(logging)

VLR_NAMESPACE_END //(vlr)
