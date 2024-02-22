#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include <functional>

#include "zstring_view.h"

namespace vlr {

namespace assert {

class Callbacks
{
public:
	using FHandleCheckFailure = std::function<void(vlr::tzstring_view svzFailureMessage)>;

	FHandleCheckFailure m_fHandleCheckFailure;

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

inline void HandleCheckFailure(vlr::tzstring_view svzFailureMessage)
{
	const auto& fHandleCheckFailure = Callbacks::getSharedInstance().m_fHandleCheckFailure;
	if (fHandleCheckFailure)
	{
		return fHandleCheckFailure(svzFailureMessage);
	}

	// Note: This works poorly when not compiling with MFC; figure out something better for default
#ifdef _MFC_VER
	ASSERT(0);
#endif
}

} // namespace assert

} // namespace vlr
