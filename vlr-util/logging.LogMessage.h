#pragma once

#include <atomic>

#include "config.h"

#include "logging.MessageContext.h"
#include "util.choice.h"
#include "util.convert.StringConversion.h"
#include "formatpf.h"

#ifndef VLR_CONSUMER_IMPL_LOGGING_CALLBACKS
#include "logging.LogMessage.DefaultImpl.h"
#else
#include "logging.Callbacks.h"
#endif

namespace vlr {

namespace logging {

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
#elif defined(VLR_CONSUMER_BOOTSTRAP_LOGGING_CALLBACKS)
	VLR_CONSUMER_BOOTSTRAP_LOGGING_CALLBACKS;
#endif
}

// Note: Not returning the formatted message eliminates some potential (spurious) static analysis warnings, if not utilized

#ifdef VLR_LOGGING_RETURN_FORMATTED_MESSAGE

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

#define VLR_LOGGING_FORMATTED_MESSAGE_RESULT_EMPTY {}
#define VLR_LOGGING_FORMATTED_MESSAGE_RESULT sMessage

#else

template <typename T>
struct TFormatResult
{
	using type = void;
};

#define VLR_LOGGING_FORMATTED_MESSAGE_RESULT_EMPTY
#define VLR_LOGGING_FORMATTED_MESSAGE_RESULT

#endif

#ifdef VLR_CONFIG_LOGGING_ALLOW_EXCEPTIONS
#define VLR_LogMossage_ExceptionSpec
#define VLR_LogMessage_Rethrow throw;
#else
#define VLR_LogMossageExceptionSpec noexcept
#define VLR_LogMessage_Rethrow
#endif

template< typename TString >
inline auto LogMessage(const CMessageContext& oMessageContext, const TString& sMessage) VLR_LogMossageExceptionSpec
-> typename TFormatResult<TString>::type
{
	try
	{
		BootstrapCallbacksOnce();

		SResult sr;
		const auto& oCallbacks = Callbacks::getSharedInstance();

		sr = oCallbacks.m_fCheckCouldMessageBeLogged(oMessageContext);
		if (sr != SResult::Success)
		{
			return VLR_LOGGING_FORMATTED_MESSAGE_RESULT_EMPTY;
		}

		/*sr =*/ oCallbacks.m_fLogMessage(oMessageContext, util::Convert::ToStdString(sMessage));

		return VLR_LOGGING_FORMATTED_MESSAGE_RESULT;
	}
	catch (...)
	{
		VLR_LogMessage_Rethrow
	}
}

template< typename TFormatString, typename... Arg >
inline auto LogMessagePF(const CMessageContext& oMessageContext, TFormatString svFormatString, Arg&&... args) VLR_LogMossageExceptionSpec
-> typename TFormatResult<TFormatString>::type
{
	try
	{
		BootstrapCallbacksOnce();

		SResult sr;
		const auto& oCallbacks = Callbacks::getSharedInstance();

		sr = oCallbacks.m_fCheckCouldMessageBeLogged(oMessageContext);
		if (sr != SResult::Success)
		{
			return VLR_LOGGING_FORMATTED_MESSAGE_RESULT_EMPTY;
		}

		auto sMessage = formatpf(svFormatString, std::forward<Arg>(args)...);

		/*sr =*/ oCallbacks.m_fLogMessage(oMessageContext, util::Convert::ToStdString(sMessage));

		return VLR_LOGGING_FORMATTED_MESSAGE_RESULT;
	}
	catch (...)
	{
		VLR_LogMessage_Rethrow
	}
}

template< typename TFormatString, typename... Arg >
inline auto LogMessageFmt(const CMessageContext& oMessageContext, TFormatString svFormatString, Arg&&... args) VLR_LogMossageExceptionSpec
-> typename TFormatResult<TFormatString>::type
{
	try
	{
		BootstrapCallbacksOnce();

		SResult sr;
		const auto& oCallbacks = Callbacks::getSharedInstance();

		sr = oCallbacks.m_fCheckCouldMessageBeLogged(oMessageContext);
		if (sr != SResult::Success)
		{
			return VLR_LOGGING_FORMATTED_MESSAGE_RESULT_EMPTY;
		}

		auto sMessage = fmt::format(svFormatString, std::forward<Arg>(args)...);

		/*sr =*/ oCallbacks.m_fLogMessage(oMessageContext, util::Convert::ToStdString(sMessage));

		return VLR_LOGGING_FORMATTED_MESSAGE_RESULT;
	}
	catch (...)
	{
		VLR_LogMessage_Rethrow
	}
}

#undef VLR_LOGGING_FORMATTED_MESSAGE_RESULT_EMPTY
#undef VLR_LOGGING_FORMATTED_MESSAGE_RESULT
#undef VLR_LogMossage_ExceptionSpec
#undef VLR_LogMessage_Rethrow

} // namespace logging

} // namespace vlr
