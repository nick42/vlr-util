#pragma once

#include <functional>

#include "config.h"
#include "RetrySettings.h"

namespace vlr {

namespace util {

template <typename TResult>
class CCallWithAutoRetrySettings
{
public:
	std::function<TResult()> m_fCall;
	std::function<bool(const TResult&)> m_fShouldRetryCall;
	CRetrySettings m_oRetrySettings;
};

template <typename TResult>
TResult CallWithAutoRetry(const CCallWithAutoRetrySettings<TResult>& oCallSettings)
{
}

} // namespace util

} // namespace vlr
