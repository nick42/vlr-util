#include "pch.h"
#include "AssertionHandling_Config.h"

namespace vlr {

namespace assert {

Callbacks& Callbacks::getSharedInstanceMutable()
{
	static auto theInstance = Callbacks{};
	return theInstance;
}

} // namespace assert

} // namespace vlr
