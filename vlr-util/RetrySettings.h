#pragma once

#include <chrono>

#include "config.h"

namespace vlr {

namespace RetrySettings {

namespace RetryDisposition {

enum ERetryDisposition
{
	Unknown,
	NoRetry,
	LinearDelay,
};

} // namespace RetryDisposition

} // namespace RetrySettings

class CRetrySettings
{
public:
	RetrySettings::RetryDisposition::ERetryDisposition m_eRetryDisposition = RetrySettings::RetryDisposition::NoRetry;

	uint32_t m_nRetryMax = 0;
	std::chrono::milliseconds m_durRetryDelayMS;

};

} // namespace vlr
