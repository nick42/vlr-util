#include "pch.h"
#include "logging.Callbacks.h"

namespace vlr {

namespace logging {

Callbacks& Callbacks::getSharedInstanceMutable()
{
	static auto theInstance = Callbacks{};
	return theInstance;
}

} // namespace logging

} // namespace vlr
