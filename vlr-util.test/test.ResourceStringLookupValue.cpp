#include "pch.h"

#include "vlr-util/ResourceStringLookupValue.h"
#include "vlr-util/StringCompare.h"

TEST(ResourceStringLookupValue, general)
{
	auto oStringCompare = vlr::StringCompare::CS();

	constexpr auto oValue = vlr::CResourceStringLookupValue{ _T("test") };
	EXPECT_TRUE(oStringCompare.AreEqual(oValue.m_svzFormatValue, _T("test")));
	EXPECT_EQ(oValue.m_nHashCRC, 0xd87f7e0c);
}
