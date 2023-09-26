#include "pch.h"

#include "vlr-util/util.range_checked_cast.h"

#include <limits>

#include "vlr-util/ModuleContext.Compilation.h"

TEST_CASE("util_range_checked_cast: general")
{
	using namespace vlr::util;

	// Note: Out of range checks will trip an assert in a debug build

	CHECK(range_checked_cast<int>(1UL) == 1);

	if constexpr (vlr::ModuleContext::Compilation::IsBuildType_Release())
	{
		CHECK(range_checked_cast<unsigned int>(std::numeric_limits<int>::min()) == unsigned int{});
	}
}
