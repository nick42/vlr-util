#include "pch.h"

#include "vlr-util/ActionOnDestruction.h"

TEST(ActionOnDestruction, general)
{
	int nValue = 42;
	{
		auto oOnDestroy_ClearValue = vlr::MakeActionOnDestruction([&] { nValue = 0; });
		ASSERT_EQ(nValue, 42);
	}
	ASSERT_EQ(nValue, 0);
}
