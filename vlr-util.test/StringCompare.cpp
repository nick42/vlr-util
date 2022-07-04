#include "pch.h"

#include "vlr-util/StringCompare.h"
#include "vlr-util/zstring_view.h"

static const auto pcaszNULL = nullptr;
static const auto pcaszBlank = "";
static const auto pcaszNotBlank = "value";
static const auto pcwszNULL = L"";
static const auto pcwszBlank = L"";
static const auto pcwszNotBlank = L"value";

static const auto saEmpty = std::string{};
static const auto saBlank = std::string{ "" };
static const auto saNotBlank = std::string{ "value" };
static const auto swEmpty = std::wstring{};
static const auto swBlank = std::wstring{ L"" };
static const auto swNotBlank = std::wstring{ L"value" };

static const auto svazEmpty = vlr::zstring_view{};
static const auto svazBlank = vlr::zstring_view{ "" };
static const auto svazNotBlank = vlr::zstring_view{ "value" };
static const auto svwzEmpty = vlr::wzstring_view{};
static const auto svwzBlank = vlr::wzstring_view{ L"" };
static const auto svwzNotBlank = vlr::wzstring_view{ L"value" };

const auto oStringCompareCS = vlr::StringCompare::CS();
const auto oStringCompareCI = vlr::StringCompare::CI();

TEST(StringCompare, asStringViewCompatType)
{
	static_assert(vlr::StringCompare::detail::isCompatTypeForString<const char*>());
	static_assert(vlr::StringCompare::detail::isCompatTypeForString<std::string_view>());
	static_assert(vlr::StringCompare::detail::isCompatTypeForString<std::string>());
	static_assert(vlr::StringCompare::detail::isCompatTypeForString<vlr::zstring_view>());

	static_assert(vlr::StringCompare::detail::isCompatTypeForWString<const wchar_t*>());
	static_assert(vlr::StringCompare::detail::isCompatTypeForWString<std::wstring_view>());
	static_assert(vlr::StringCompare::detail::isCompatTypeForWString<std::wstring>());
	static_assert(vlr::StringCompare::detail::isCompatTypeForWString<vlr::wzstring_view>());

	auto fTestForStringViewCompat = [](const auto& tValue, bool bIsBlank)
	{
		const auto& tValueCompat = vlr::StringCompare::asStringViewCompatType(tValue);
		auto svValue = std::string_view{ tValueCompat };
		EXPECT_EQ(svValue.empty(), bIsBlank);
	};

	fTestForStringViewCompat(pcaszNULL, true);
	fTestForStringViewCompat(pcaszBlank, true);
	fTestForStringViewCompat(pcaszNotBlank, false);
	fTestForStringViewCompat(saEmpty, true);
	fTestForStringViewCompat(saBlank, true);
	fTestForStringViewCompat(saNotBlank, false);
	fTestForStringViewCompat(svazEmpty, true);
	fTestForStringViewCompat(svazBlank, true);
	fTestForStringViewCompat(svazNotBlank, false);

	auto fTestForWStringViewCompat = [](const auto& tValue, bool bIsBlank)
	{
		const auto& tValueCompat = vlr::StringCompare::asWStringViewCompatType(tValue);
		auto svValue = std::wstring_view{ tValueCompat };
		EXPECT_EQ(svValue.empty(), bIsBlank);
	};

	fTestForWStringViewCompat(pcwszNULL, true);
	fTestForWStringViewCompat(pcwszBlank, true);
	fTestForWStringViewCompat(pcwszNotBlank, false);
	fTestForWStringViewCompat(swEmpty, true);
	fTestForWStringViewCompat(swBlank, true);
	fTestForWStringViewCompat(swNotBlank, false);
	fTestForWStringViewCompat(svwzEmpty, true);
	fTestForWStringViewCompat(svwzBlank, true);
	fTestForWStringViewCompat(svwzNotBlank, false);
}

TEST(StringCompare, IsBlank)
{
	auto fTestForStringCompare = [&](const auto& oStringCompare)
	{
		EXPECT_EQ(oStringCompare.IsBlank(pcaszNULL), true);
		EXPECT_EQ(oStringCompare.IsBlank(pcaszBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(pcaszNotBlank), false);
		EXPECT_EQ(oStringCompare.IsBlank(pcwszNULL), true);
		EXPECT_EQ(oStringCompare.IsBlank(pcwszBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(pcwszNotBlank), false);

		EXPECT_EQ(oStringCompare.IsBlank(saEmpty), true);
		EXPECT_EQ(oStringCompare.IsBlank(saBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(saNotBlank), false);
		EXPECT_EQ(oStringCompare.IsBlank(swEmpty), true);
		EXPECT_EQ(oStringCompare.IsBlank(swBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(swNotBlank), false);

		EXPECT_EQ(oStringCompare.IsBlank(svazEmpty), true);
		EXPECT_EQ(oStringCompare.IsBlank(svazBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(svazNotBlank), false);
		EXPECT_EQ(oStringCompare.IsBlank(svwzEmpty), true);
		EXPECT_EQ(oStringCompare.IsBlank(svwzBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(svwzNotBlank), false);
	};

	fTestForStringCompare(oStringCompareCS);
	fTestForStringCompare(oStringCompareCI);
}

TEST(StringCompare, AreEqual)
{
	auto fTestAreEqual = [&](const auto& oStringCompare, const auto& lhs, const auto& rhs, bool bAreEqual)
	{
		EXPECT_EQ(oStringCompare.AreEqual(lhs, rhs), bAreEqual);
	};

	auto fTestAreEqual_BasicValueCombinations = [&](const auto& oStringCompare)
	{
		auto fTestAreEqual_a_Blank = [&](const auto& tValue)
		{
			fTestAreEqual(oStringCompare, tValue, pcaszNULL, true);
			fTestAreEqual(oStringCompare, tValue, pcaszBlank, true);
			fTestAreEqual(oStringCompare, tValue, pcaszNotBlank, false);
			fTestAreEqual(oStringCompare, tValue, saEmpty, true);
			fTestAreEqual(oStringCompare, tValue, saBlank, true);
			fTestAreEqual(oStringCompare, tValue, saNotBlank, false);
			fTestAreEqual(oStringCompare, tValue, svazEmpty, true);
			fTestAreEqual(oStringCompare, tValue, svazBlank, true);
			fTestAreEqual(oStringCompare, tValue, svazNotBlank, false);
		};
		fTestAreEqual_a_Blank(pcaszNULL);
		fTestAreEqual_a_Blank(pcaszBlank);
		fTestAreEqual_a_Blank(saEmpty);
		fTestAreEqual_a_Blank(saBlank);
		fTestAreEqual_a_Blank(svazEmpty);
		fTestAreEqual_a_Blank(svazBlank);

		auto fTestAreEqual_a_NotBlank = [&](const auto& tValue)
		{
			fTestAreEqual(oStringCompare, tValue, pcaszNULL, false);
			fTestAreEqual(oStringCompare, tValue, pcaszBlank, false);
			fTestAreEqual(oStringCompare, tValue, pcaszNotBlank, true);
			fTestAreEqual(oStringCompare, tValue, saEmpty, false);
			fTestAreEqual(oStringCompare, tValue, saBlank, false);
			fTestAreEqual(oStringCompare, tValue, saNotBlank, true);
			fTestAreEqual(oStringCompare, tValue, svazEmpty, false);
			fTestAreEqual(oStringCompare, tValue, svazBlank, false);
			fTestAreEqual(oStringCompare, tValue, svazNotBlank, true);
		};
		fTestAreEqual_a_NotBlank(pcaszNotBlank);
		fTestAreEqual_a_NotBlank(saNotBlank);
		fTestAreEqual_a_NotBlank(svazNotBlank);

		auto fTestAreEqual_w_Blank = [&](const auto& tValue)
		{
			fTestAreEqual(oStringCompare, tValue, pcwszNULL, true);
			fTestAreEqual(oStringCompare, tValue, pcwszBlank, true);
			fTestAreEqual(oStringCompare, tValue, pcwszNotBlank, false);
			fTestAreEqual(oStringCompare, tValue, swEmpty, true);
			fTestAreEqual(oStringCompare, tValue, swBlank, true);
			fTestAreEqual(oStringCompare, tValue, swNotBlank, false);
			fTestAreEqual(oStringCompare, tValue, svwzEmpty, true);
			fTestAreEqual(oStringCompare, tValue, svwzBlank, true);
			fTestAreEqual(oStringCompare, tValue, svwzNotBlank, false);
		};
		fTestAreEqual_w_Blank(pcwszNULL);
		fTestAreEqual_w_Blank(pcwszBlank);
		fTestAreEqual_w_Blank(swEmpty);
		fTestAreEqual_w_Blank(swBlank);
		fTestAreEqual_w_Blank(svwzEmpty);
		fTestAreEqual_w_Blank(svwzBlank);

		auto fTestAreEqual_w_NotBlank = [&](const auto& tValue)
		{
			fTestAreEqual(oStringCompare, tValue, pcwszNULL, false);
			fTestAreEqual(oStringCompare, tValue, pcwszBlank, false);
			fTestAreEqual(oStringCompare, tValue, pcwszNotBlank, true);
			fTestAreEqual(oStringCompare, tValue, swEmpty, false);
			fTestAreEqual(oStringCompare, tValue, swBlank, false);
			fTestAreEqual(oStringCompare, tValue, swNotBlank, true);
			fTestAreEqual(oStringCompare, tValue, svwzEmpty, false);
			fTestAreEqual(oStringCompare, tValue, svwzBlank, false);
			fTestAreEqual(oStringCompare, tValue, svwzNotBlank, true);
		};
		fTestAreEqual_w_NotBlank(pcwszNotBlank);
		fTestAreEqual_w_NotBlank(swNotBlank);
		fTestAreEqual_w_NotBlank(svwzNotBlank);
	};

	fTestAreEqual_BasicValueCombinations(oStringCompareCS);
	fTestAreEqual_BasicValueCombinations(oStringCompareCI);

#define AVALUE_LOWER "value"
#define AVALUE_UPPER "VALUE"
#define AVALUE_MIXED "VaLuE"
#define WVALUE_LOWER L"value"
#define WVALUE_UPPER L"VALUE"
#define WVALUE_MIXED L"VaLuE"

	static const auto pcaszLower = AVALUE_LOWER;
	static const auto saLower = std::string{ AVALUE_LOWER };
	static const auto svazLower = vlr::zstring_view{ AVALUE_LOWER };
	static const auto pcaszUpper = AVALUE_UPPER;
	static const auto saUpper = std::string{ AVALUE_UPPER };
	static const auto svazUpper = vlr::zstring_view{ AVALUE_UPPER };
	static const auto pcaszMixed = AVALUE_MIXED;
	static const auto saMixed = std::string{ AVALUE_MIXED };
	static const auto svazMixed = vlr::zstring_view{ AVALUE_MIXED };

	static const auto pcwszLower = WVALUE_LOWER;
	static const auto swLower = std::wstring{ WVALUE_LOWER };
	static const auto svwzLower = vlr::wzstring_view{ WVALUE_LOWER };
	static const auto pcwszUpper= WVALUE_UPPER;
	static const auto swUpper= std::wstring{ WVALUE_UPPER };
	static const auto svwzUpper= vlr::wzstring_view{ WVALUE_UPPER };
	static const auto pcwszMixed = WVALUE_MIXED;
	static const auto swMixed = std::wstring{ WVALUE_MIXED };
	static const auto svwzMixed = vlr::wzstring_view{ WVALUE_MIXED };

#undef AVALUE_LOWER
#undef AVALUE_UPPER
#undef AVALUE_MIXED
#undef WVALUE_LOWER
#undef WVALUE_UPPER
#undef WVALUE_MIXED

	// Everything should be equal for case insensitive compare

	auto fTestAreEqual_a_CaseInsensitive = [&](const auto& tValue)
	{
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, pcaszLower), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, saLower), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, svazLower), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, pcaszUpper), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, saUpper), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, svazUpper), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, pcaszMixed), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, saMixed), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, svazMixed), true);
	};
	fTestAreEqual_a_CaseInsensitive(pcaszLower);
	fTestAreEqual_a_CaseInsensitive(saLower);
	fTestAreEqual_a_CaseInsensitive(svazLower);
	fTestAreEqual_a_CaseInsensitive(pcaszUpper);
	fTestAreEqual_a_CaseInsensitive(saUpper);
	fTestAreEqual_a_CaseInsensitive(svazUpper);
	fTestAreEqual_a_CaseInsensitive(pcaszMixed);
	fTestAreEqual_a_CaseInsensitive(saMixed);
	fTestAreEqual_a_CaseInsensitive(svazMixed);

	auto fTestAreEqual_w_CaseInsensitive = [&](const auto& tValue)
	{
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, pcwszLower), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, swLower), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, svwzLower), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, pcwszUpper), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, swUpper), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, svwzUpper), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, pcwszMixed), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, swMixed), true);
		EXPECT_EQ(oStringCompareCI.AreEqual(tValue, svwzMixed), true);
	};
	fTestAreEqual_w_CaseInsensitive(pcwszLower);
	fTestAreEqual_w_CaseInsensitive(swLower);
	fTestAreEqual_w_CaseInsensitive(svwzLower);
	fTestAreEqual_w_CaseInsensitive(pcwszUpper);
	fTestAreEqual_w_CaseInsensitive(swUpper);
	fTestAreEqual_w_CaseInsensitive(svwzUpper);
	fTestAreEqual_w_CaseInsensitive(pcwszMixed);
	fTestAreEqual_w_CaseInsensitive(swMixed);
	fTestAreEqual_w_CaseInsensitive(svwzMixed);

	// Case sensitive is more complex
	// Note: Not going to test all combinations (n^m); going to use zstring_view as baseline

	auto fExpectCompareResult_CaseSensitive = [&](const auto& tlhs, const auto& trhs, bool bAreEqual)
	{
		EXPECT_EQ(oStringCompareCS.AreEqual(tlhs, trhs), bAreEqual);
	};
	fExpectCompareResult_CaseSensitive(svazLower, pcaszLower, true);
	fExpectCompareResult_CaseSensitive(svazLower, saLower, true);
	fExpectCompareResult_CaseSensitive(svazLower, svazLower, true);
	fExpectCompareResult_CaseSensitive(svazLower, pcaszUpper, false);
	fExpectCompareResult_CaseSensitive(svazLower, saUpper, false);
	fExpectCompareResult_CaseSensitive(svazLower, svazUpper, false);
	fExpectCompareResult_CaseSensitive(svazLower, pcaszMixed, false);
	fExpectCompareResult_CaseSensitive(svazLower, saMixed, false);
	fExpectCompareResult_CaseSensitive(svazLower, svazMixed, false);

	fExpectCompareResult_CaseSensitive(svwzLower, pcwszLower, true);
	fExpectCompareResult_CaseSensitive(svwzLower, swLower, true);
	fExpectCompareResult_CaseSensitive(svwzLower, svwzLower, true);
	fExpectCompareResult_CaseSensitive(svwzLower, pcwszUpper, false);
	fExpectCompareResult_CaseSensitive(svwzLower, swUpper, false);
	fExpectCompareResult_CaseSensitive(svwzLower, svwzUpper, false);
	fExpectCompareResult_CaseSensitive(svwzLower, pcwszMixed, false);
	fExpectCompareResult_CaseSensitive(svwzLower, swMixed, false);
	fExpectCompareResult_CaseSensitive(svwzLower, svwzMixed, false);

	// Test conversion cases

	fTestAreEqual_a_CaseInsensitive(pcwszLower);
	fTestAreEqual_a_CaseInsensitive(swLower);
	fTestAreEqual_a_CaseInsensitive(svwzLower);
	fTestAreEqual_a_CaseInsensitive(pcwszUpper);
	fTestAreEqual_a_CaseInsensitive(swUpper);
	fTestAreEqual_a_CaseInsensitive(svwzUpper);
	fTestAreEqual_a_CaseInsensitive(pcwszMixed);
	fTestAreEqual_a_CaseInsensitive(swMixed);
	fTestAreEqual_a_CaseInsensitive(svwzMixed);

	fTestAreEqual_w_CaseInsensitive(pcaszLower);
	fTestAreEqual_w_CaseInsensitive(saLower);
	fTestAreEqual_w_CaseInsensitive(svazLower);
	fTestAreEqual_w_CaseInsensitive(pcaszUpper);
	fTestAreEqual_w_CaseInsensitive(saUpper);
	fTestAreEqual_w_CaseInsensitive(svazUpper);
	fTestAreEqual_w_CaseInsensitive(pcaszMixed);
	fTestAreEqual_w_CaseInsensitive(saMixed);
	fTestAreEqual_w_CaseInsensitive(svazMixed);

	fExpectCompareResult_CaseSensitive(svwzLower, pcaszLower, true);
	fExpectCompareResult_CaseSensitive(svwzLower, saLower, true);
	fExpectCompareResult_CaseSensitive(svwzLower, svazLower, true);
	fExpectCompareResult_CaseSensitive(svwzLower, pcaszUpper, false);
	fExpectCompareResult_CaseSensitive(svwzLower, saUpper, false);
	fExpectCompareResult_CaseSensitive(svwzLower, svazUpper, false);
	fExpectCompareResult_CaseSensitive(svwzLower, pcaszMixed, false);
	fExpectCompareResult_CaseSensitive(svwzLower, saMixed, false);
	fExpectCompareResult_CaseSensitive(svwzLower, svazMixed, false);

	fExpectCompareResult_CaseSensitive(svazLower, pcwszLower, true);
	fExpectCompareResult_CaseSensitive(svazLower, swLower, true);
	fExpectCompareResult_CaseSensitive(svazLower, svwzLower, true);
	fExpectCompareResult_CaseSensitive(svazLower, pcwszUpper, false);
	fExpectCompareResult_CaseSensitive(svazLower, swUpper, false);
	fExpectCompareResult_CaseSensitive(svazLower, svwzUpper, false);
	fExpectCompareResult_CaseSensitive(svazLower, pcwszMixed, false);
	fExpectCompareResult_CaseSensitive(svazLower, swMixed, false);
	fExpectCompareResult_CaseSensitive(svazLower, svwzMixed, false);

}
