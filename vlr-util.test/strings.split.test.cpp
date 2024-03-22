#include "pch.h"

#include "vlr-util/strings.split.h"
#include "vlr-util/StringCompare.h"

using namespace vlr;

TEST(strings_split, general)
{
	static constexpr vlr::tzstring_view svzTestDirectory = _T("/etc/something");
	static constexpr vlr::tzstring_view svzTestDirectory_Element1 = _T("etc");
	static constexpr vlr::tzstring_view svzTestDirectory_Element2 = _T("something");

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

TEST(strings_split, StringWithoutPossiblePrefix)
{
	static constexpr vlr::tzstring_view svzTestValue = _T("/etc/something");
	static constexpr vlr::tzstring_view svzTestPrefix_Valid = _T("/etc");
	static constexpr vlr::tzstring_view svzTestPrefix_Invalid = _T("/usr");

	static const auto oStringCompareCS = StringCompare::CS();

	{
		auto svValueWithoutPrefix = strings::StringWithoutPossiblePrefix(svzTestValue, svzTestPrefix_Valid);
		ASSERT_TRUE(oStringCompareCS.AreEqual(svValueWithoutPrefix, _T("/something")));
	}
	{
		auto svValueWithoutPrefix = strings::StringWithoutPossiblePrefix(svzTestValue, svzTestPrefix_Invalid);
		ASSERT_TRUE(oStringCompareCS.AreEqual(svValueWithoutPrefix, _T("/etc/something")));
	}
	{
		auto svValueWithoutPrefix = strings::StringWithoutPossiblePrefix(svzTestValue, svzTestPrefix_Invalid);
		ASSERT_TRUE(oStringCompareCS.AreEqual(svValueWithoutPrefix, _T("/etc/something")));
	}
}

TEST(strings_split, StringWithoutPossiblePostfix)
{
	static constexpr vlr::tzstring_view svzTestValue = _T("/etc/something");
	static constexpr vlr::tzstring_view svzTestPostfix_Valid = _T("/something");
	static constexpr vlr::tzstring_view svzTestPostfix_Invalid = _T("/else");

	static const auto oStringCompareCS = StringCompare::CS();

	{
		auto svValueWithoutPrefix = strings::StringWithoutPossiblePostfix(svzTestValue, svzTestPostfix_Valid);
		ASSERT_TRUE(oStringCompareCS.AreEqual(svValueWithoutPrefix, _T("/etc")));
	}
	{
		auto svValueWithoutPrefix = strings::StringWithoutPossiblePostfix(svzTestValue, svzTestPostfix_Invalid);
		ASSERT_TRUE(oStringCompareCS.AreEqual(svValueWithoutPrefix, _T("/etc/something")));
	}
	{
		auto svValueWithoutPrefix = strings::StringWithoutPossiblePostfix(svzTestValue, svzTestPostfix_Invalid);
		ASSERT_TRUE(oStringCompareCS.AreEqual(svValueWithoutPrefix, _T("/etc/something")));
	}
}

TEST(strings_split, GetTrimmedStringView)
{
	static constexpr vlr::tzstring_view svzTestValue_NoTrim = _T("blah");
	static constexpr vlr::tzstring_view svzTestValue_WithPrefix = _T("\tblah");
	static constexpr vlr::tzstring_view svzTestValue_WithPostfix = _T("blah \r\n");
	static constexpr vlr::tzstring_view svzTestValue_WithBoth = _T("  blah\t \r\n");
	static constexpr vlr::tzstring_view svzTestValue_NoMatch = _T(" bl ah");
	static constexpr vlr::tzstring_view svzTestResult_Expected = _T("blah");
	static constexpr vlr::tzstring_view svzTestDelimiters = strings::DelimitersSpec<TCHAR>::GetChars_Whitespace();

	static const auto oStringCompareCS = StringCompare::CS();

	{
		auto svValue = strings::GetTrimmedStringView(svzTestValue_NoTrim, svzTestDelimiters);
		EXPECT_TRUE(oStringCompareCS.AreEqual(svValue, svzTestResult_Expected))
			<< _T("Test value which failed: ") << svzTestValue_NoTrim;
	}
	{
		auto svValue = strings::GetTrimmedStringView(svzTestValue_WithPrefix, svzTestDelimiters);
		EXPECT_TRUE(oStringCompareCS.AreEqual(svValue, svzTestResult_Expected))
			<< _T("Test value which failed: ") << svzTestValue_WithPrefix;
	}
	{
		auto svValue = strings::GetTrimmedStringView(svzTestValue_WithPostfix, svzTestDelimiters);
		EXPECT_TRUE(oStringCompareCS.AreEqual(svValue, svzTestResult_Expected))
			<< _T("Test value which failed: ") << svzTestValue_WithPostfix;
	}
	{
		auto svValue = strings::GetTrimmedStringView(svzTestValue_WithBoth, svzTestDelimiters);
		EXPECT_TRUE(oStringCompareCS.AreEqual(svValue, svzTestResult_Expected))
			<< _T("Test value which failed: ") << svzTestValue_WithBoth;
	}
	{
		auto svValue = strings::GetTrimmedStringView(svzTestValue_NoMatch, svzTestDelimiters);
		EXPECT_FALSE(oStringCompareCS.AreEqual(svValue, svzTestResult_Expected))
			<< _T("Test value which failed: ") << svzTestValue_NoMatch;
	}
}
