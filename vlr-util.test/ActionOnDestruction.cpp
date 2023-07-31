#include "pch.h"

#include "vlr-util/ActionOnDestruction.h"

TEST_CASE("ActionOnDestruction: general")
{
	int nValue = 42;
	{
		auto oOnDestroy_ClearValue = vlr::MakeActionOnDestruction([&] { nValue = 0; });
		CHECK(nValue == 42);
	}
	CHECK(nValue == 0);
}
