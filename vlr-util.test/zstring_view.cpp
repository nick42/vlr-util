#include "zstring_view.h"

#include <gtest/gtest.h>

#include "../vlr-util/zstring_view.h"

TEST(zstring_view, Base)
{
	auto svzTest = vlr::zstring_view{ "test" };
	EXPECT_EQ(svzTest.length(), 4);
	//auto svTest = std::string_view{ "test" };
}
