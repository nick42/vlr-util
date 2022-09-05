#include "pch.h"

#include "vlr-util/strings.split.h"
#include "vlr-util/StringCompare.h"

using namespace vlr;

TEST(strings_split, general)
{
	static const vlr::tzstring_view svzTestDirectory = _T("/etc/something");
	static const vlr::tzstring_view svzTestDirectory_Element1 = _T("etc");
	static const vlr::tzstring_view svzTestDirectory_Element2 = _T("something");

	static const auto oStringCompareCS = StringCompare::CS();

	SResult sr;

	{
		std::vector<vlr::tstring_view> arrStringElements;
		sr = strings::SplitStringAtDelimiter_Path(svzTestDirectory, arrStringElements);
		ASSERT_TRUE(sr.isSuccess());
		ASSERT_EQ(arrStringElements.size(), 2);
		auto nResultIndex = 0;
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element1));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element2));
	}

	{
		auto options = strings::Options_SplitStringAtDelimiter{};
		options.m_bAddEmptyElementsForPrefixDelimiters = true;

		std::vector<vlr::tstring_view> arrStringElements;
		sr = strings::SplitStringAtDelimiter_Path(svzTestDirectory, arrStringElements, options);
		ASSERT_TRUE(sr.isSuccess());
		ASSERT_EQ(arrStringElements.size(), 3);
		auto nResultIndex = 0;
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], _T("")));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element1));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element2));
	}

	{
		static const vlr::tzstring_view svzTestDirectory_ForCase = _T("/etc//something");

		auto options = strings::Options_SplitStringAtDelimiter{};
		options.m_bAddEmptyElementsForConsecutiveDelimiters = true;

		std::vector<vlr::tstring_view> arrStringElements;
		sr = strings::SplitStringAtDelimiter_Path(svzTestDirectory_ForCase, arrStringElements, options);
		ASSERT_TRUE(sr.isSuccess());
		ASSERT_EQ(arrStringElements.size(), 3);
		auto nResultIndex = 0;
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element1));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], _T("")));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element2));
	}

	{
		static const vlr::tzstring_view svzTestDirectory_ForCase = _T("/etc//something/");

		auto options = strings::Options_SplitStringAtDelimiter{};
		options.m_bAddEmptyElementsForPostfixDelimiters = true;

		std::vector<vlr::tstring_view> arrStringElements;
		sr = strings::SplitStringAtDelimiter_Path(svzTestDirectory_ForCase, arrStringElements, options);
		ASSERT_TRUE(sr.isSuccess());
		ASSERT_EQ(arrStringElements.size(), 3);
		auto nResultIndex = 0;
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element1));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], svzTestDirectory_Element2));
		ASSERT_TRUE(oStringCompareCS.AreEqual(arrStringElements[nResultIndex++], _T("")));
	}
}
