#include "pch.h"

#include "vlr-util/ActionOnDestruction.h"
#include "vlr-util/util.NonOwningSharedPtr.h"

TEST(util_NonOwningSharedPtr, general)
{
	int nValue = 42;
	auto oOnDestroy_ClearValue = vlr::MakeActionOnDestruction([&] { nValue = 0; });

	{
		auto spNonOwningOnDestroy = vlr::MakeNonOwningSharedPtr(&oOnDestroy_ClearValue);
		ASSERT_NE(spNonOwningOnDestroy, nullptr);
		ASSERT_EQ(spNonOwningOnDestroy.get(), &oOnDestroy_ClearValue);
	}
	ASSERT_EQ(nValue, 42);
}
