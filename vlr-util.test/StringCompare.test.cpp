#include "pch.h"

#include "vlr-util/StringCompare.h"
#include "vlr-util/zstring_view.h"

static constexpr auto pcaszNULL = nullptr;
static constexpr auto pcaszBlank = "";
static constexpr auto pcaszNotBlank = "value";
static constexpr auto pcwszNULL = L"";
static constexpr auto pcwszBlank = L"";
static constexpr auto pcwszNotBlank = L"value";

static const auto saEmpty = std::string{};
static const auto saBlank = std::string{ "" };
static const auto saNotBlank = std::string{ "value" };
static const auto swEmpty = std::wstring{};
static const auto swBlank = std::wstring{ L"" };
static const auto swNotBlank = std::wstring{ L"value" };

static constexpr auto svazEmpty = vlr::zstring_view{};
static constexpr auto svazBlank = vlr::zstring_view{ "" };
static constexpr auto svazNotBlank = vlr::zstring_view{ "value" };
static constexpr auto svwzEmpty = vlr::wzstring_view{};
static constexpr auto svwzBlank = vlr::wzstring_view{ L"" };
static constexpr auto svwzNotBlank = vlr::wzstring_view{ L"value" };

static constexpr auto svazShorter = vlr::zstring_view{ "valu" };
static constexpr auto svwzShorter = vlr::wzstring_view{ L"valu" };
static constexpr auto svazLonger = vlr::zstring_view{ "values" };
static constexpr auto svwzLonger = vlr::wzstring_view{ L"values" };
static constexpr auto svazDifferent = vlr::zstring_view{ "other" };
static constexpr auto svwzDifferent = vlr::wzstring_view{ L"other" };

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t

static const auto bsEmpty = _bstr_t{};
static const auto bsBlank = _bstr_t{ L"" };
static const auto bsNotBlank = _bstr_t{ L"value" };

#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR

static const auto bs2Empty = ATL::CComBSTR{};
static const auto bs2Blank = ATL::CComBSTR{ L"" };
static const auto bs2NotBlank = ATL::CComBSTR{ L"value" };

#endif

#define AVALUE_LOWER "value"
#define AVALUE_UPPER "VALUE"
#define AVALUE_MIXED "VaLuE"
#define WVALUE_LOWER L"value"
#define WVALUE_UPPER L"VALUE"
#define WVALUE_MIXED L"VaLuE"

static constexpr auto pcaszLower = AVALUE_LOWER;
static const auto saLower = std::string{ AVALUE_LOWER };
static constexpr auto svazLower = vlr::zstring_view{ AVALUE_LOWER };
static constexpr auto pcaszUpper = AVALUE_UPPER;
static const auto saUpper = std::string{ AVALUE_UPPER };
static constexpr auto svazUpper = vlr::zstring_view{ AVALUE_UPPER };
static constexpr auto pcaszMixed = AVALUE_MIXED;
static const auto saMixed = std::string{ AVALUE_MIXED };
static constexpr auto svazMixed = vlr::zstring_view{ AVALUE_MIXED };

static constexpr auto pcwszLower = WVALUE_LOWER;
static const auto swLower = std::wstring{ WVALUE_LOWER };
static constexpr auto svwzLower = vlr::wzstring_view{ WVALUE_LOWER };
static constexpr auto pcwszUpper = WVALUE_UPPER;
static const auto swUpper = std::wstring{ WVALUE_UPPER };
static constexpr auto svwzUpper = vlr::wzstring_view{ WVALUE_UPPER };
static constexpr auto pcwszMixed = WVALUE_MIXED;
static const auto swMixed = std::wstring{ WVALUE_MIXED };
static constexpr auto svwzMixed = vlr::wzstring_view{ WVALUE_MIXED };

#undef AVALUE_LOWER
#undef AVALUE_UPPER
#undef AVALUE_MIXED
#undef WVALUE_LOWER
#undef WVALUE_UPPER
#undef WVALUE_MIXED

static constexpr auto oStringCompareCS = vlr::StringCompare::CS();
static constexpr auto oStringCompareCI = vlr::StringCompare::CI();

TEST(StringCompare, asAStringViewCompatType)
{
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_astring_view<const char*>());
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_astring_view<std::string_view>());
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_astring_view<std::string>());
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_astring_view<vlr::zstring_view>());

	static_assert(vlr::StringCompare::detail::isCompatTypeFor_wstring_view<const wchar_t*>());
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_wstring_view<std::wstring_view>());
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_wstring_view<std::wstring>());
	static_assert(vlr::StringCompare::detail::isCompatTypeFor_wstring_view<vlr::wzstring_view>());

	auto fTestForStringViewCompat = [](const auto& tValue, bool bIsBlank)
	{
		const auto& tValueCompat = vlr::StringCompare::asAStringViewCompatType(tValue);
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

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
	fTestForWStringViewCompat(bsEmpty, true);
	fTestForWStringViewCompat(bsBlank, true);
	fTestForWStringViewCompat(bsNotBlank, false);
#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
	fTestForWStringViewCompat(bs2Empty, true);
	fTestForWStringViewCompat(bs2Blank, true);
	fTestForWStringViewCompat(bs2NotBlank, false);
#endif
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

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
		EXPECT_EQ(oStringCompare.IsBlank(bsEmpty), true);
		EXPECT_EQ(oStringCompare.IsBlank(bsBlank), true);
		EXPECT_EQ(oStringCompare.IsBlank(bsNotBlank), false);
#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
		EXPECT_EQ(oStringCompare.IsBlank(bs2Empty), true);
		EXPECT_EQ(oStringCompare.IsBlank(bs2Blank), true);
		EXPECT_EQ(oStringCompare.IsBlank(bs2NotBlank), false);
#endif
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

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
			fTestAreEqual(oStringCompare, tValue, bsEmpty, true);
			fTestAreEqual(oStringCompare, tValue, bsBlank, true);
			fTestAreEqual(oStringCompare, tValue, bsNotBlank, false);
#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
			fTestAreEqual(oStringCompare, tValue, bs2Empty, true);
			fTestAreEqual(oStringCompare, tValue, bs2Blank, true);
			fTestAreEqual(oStringCompare, tValue, bs2NotBlank, false);
#endif
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

#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
			fTestAreEqual(oStringCompare, tValue, bsEmpty, false);
			fTestAreEqual(oStringCompare, tValue, bsBlank, false);
			fTestAreEqual(oStringCompare, tValue, bsNotBlank, true);
#endif

#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
			fTestAreEqual(oStringCompare, tValue, bs2Empty, false);
			fTestAreEqual(oStringCompare, tValue, bs2Blank, false);
			fTestAreEqual(oStringCompare, tValue, bs2NotBlank, true);
#endif
		};
		fTestAreEqual_w_NotBlank(pcwszNotBlank);
		fTestAreEqual_w_NotBlank(swNotBlank);
		fTestAreEqual_w_NotBlank(svwzNotBlank);
	};

	fTestAreEqual_BasicValueCombinations(oStringCompareCS);
	fTestAreEqual_BasicValueCombinations(oStringCompareCI);

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

	// Any of the test strings should compare as not-equal with any explicitly different values

	auto fTestNotEqual_DifferentValues = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tValue)
	{
		fTestAreEqual(oStringCompare, tValue, svazShorter, false);
		fTestAreEqual(oStringCompare, tValue, svazLonger, false);
		fTestAreEqual(oStringCompare, tValue, svwzShorter, false);
		fTestAreEqual(oStringCompare, tValue, svwzLonger, false);
		fTestAreEqual(oStringCompare, tValue, svwzDifferent, false);
		fTestAreEqual(oStringCompare, tValue, svwzDifferent, false);
	};
	auto fTestNotEqual_DifferentValues_AnyCompare = [&](const auto& tValue)
	{
		fTestNotEqual_DifferentValues(oStringCompareCS, tValue);
		fTestNotEqual_DifferentValues(oStringCompareCI, tValue);
	};
	fTestNotEqual_DifferentValues_AnyCompare(svazEmpty);
	fTestNotEqual_DifferentValues_AnyCompare(svwzEmpty);
	fTestNotEqual_DifferentValues_AnyCompare(svazBlank);
	fTestNotEqual_DifferentValues_AnyCompare(svwzBlank);
	fTestNotEqual_DifferentValues_AnyCompare(svazNotBlank);
	fTestNotEqual_DifferentValues_AnyCompare(svwzNotBlank);

}

TEST(StringCompare, Compare)
{
	auto fTestCompareLeftIsEqual = [&](const auto& oStringCompare, const auto& lhs, const auto& rhs)
	{
		EXPECT_EQ(oStringCompare.Compare(lhs, rhs), 0);
	};
	auto fTestCompareLeftIsLess = [&](const auto& oStringCompare, const auto& lhs, const auto& rhs)
	{
		EXPECT_LT(oStringCompare.Compare(lhs, rhs), 0);
	};
	auto fTestCompareLeftIsGreater = [&](const auto& oStringCompare, const auto& lhs, const auto& rhs)
	{
		EXPECT_GT(oStringCompare.Compare(lhs, rhs), 0);
	};
	auto fTestCompareLeftIsGreaterOrEqual = [&](const auto& oStringCompare, const auto& lhs, const auto& rhs)
	{
		EXPECT_GE(oStringCompare.Compare(lhs, rhs), 0);
	};

	auto fTestCompare_BasicValueCombinations = [&](const auto& oStringCompare)
	{
		// For lhs blank:
		// - If rhs blank, =
		// - If rhs non-blank, <
		auto fTestCompare_a_Blank = [&](const auto& tValue)
		{
			fTestCompareLeftIsEqual(oStringCompare, tValue, pcaszNULL);
			fTestCompareLeftIsEqual(oStringCompare, tValue, pcaszBlank);
			fTestCompareLeftIsLess(oStringCompare, tValue, pcaszNotBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, saEmpty);
			fTestCompareLeftIsEqual(oStringCompare, tValue, saBlank);
			fTestCompareLeftIsLess(oStringCompare, tValue, saNotBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, svazEmpty);
			fTestCompareLeftIsEqual(oStringCompare, tValue, svazBlank);
			fTestCompareLeftIsLess(oStringCompare, tValue, svazNotBlank);
		};
		fTestCompare_a_Blank(pcaszNULL);
		fTestCompare_a_Blank(pcaszBlank);
		fTestCompare_a_Blank(saEmpty);
		fTestCompare_a_Blank(saBlank);
		fTestCompare_a_Blank(svazEmpty);
		fTestCompare_a_Blank(svazBlank);

		// For lhs non-blank:
		// - If rhs blank, >
		// - If rhs non-blank, =
		auto fTestCompare_a_NotBlank = [&](const auto& tValue)
		{
			fTestCompareLeftIsGreater(oStringCompare, tValue, pcaszNULL);
			fTestCompareLeftIsGreater(oStringCompare, tValue, pcaszBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, pcaszNotBlank);
			fTestCompareLeftIsGreater(oStringCompare, tValue, saEmpty);
			fTestCompareLeftIsGreater(oStringCompare, tValue, saBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, saNotBlank);
			fTestCompareLeftIsGreater(oStringCompare, tValue, svazEmpty);
			fTestCompareLeftIsGreater(oStringCompare, tValue, svazBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, svazNotBlank);
		};
		fTestCompare_a_NotBlank(pcaszNotBlank);
		fTestCompare_a_NotBlank(saNotBlank);
		fTestCompare_a_NotBlank(svazNotBlank);

		auto fTestCompare_w_Blank = [&](const auto& tValue)
		{
			fTestCompareLeftIsEqual(oStringCompare, tValue, pcwszNULL);
			fTestCompareLeftIsEqual(oStringCompare, tValue, pcwszBlank);
			fTestCompareLeftIsLess(oStringCompare, tValue, pcwszNotBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, swEmpty);
			fTestCompareLeftIsEqual(oStringCompare, tValue, swBlank);
			fTestCompareLeftIsLess(oStringCompare, tValue, swNotBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, svwzEmpty);
			fTestCompareLeftIsEqual(oStringCompare, tValue, svwzBlank);
			fTestCompareLeftIsLess(oStringCompare, tValue, svwzNotBlank);
		};
		fTestCompare_w_Blank(pcwszNULL);
		fTestCompare_w_Blank(pcwszBlank);
		fTestCompare_w_Blank(swEmpty);
		fTestCompare_w_Blank(swBlank);
		fTestCompare_w_Blank(svwzEmpty);
		fTestCompare_w_Blank(svwzBlank);

		auto fTestCompre_w_NotBlank = [&](const auto& tValue)
		{
			fTestCompareLeftIsGreater(oStringCompare, tValue, pcwszNULL);
			fTestCompareLeftIsGreater(oStringCompare, tValue, pcwszBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, pcwszNotBlank);
			fTestCompareLeftIsGreater(oStringCompare, tValue, swEmpty);
			fTestCompareLeftIsGreater(oStringCompare, tValue, swBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, swNotBlank);
			fTestCompareLeftIsGreater(oStringCompare, tValue, svwzEmpty);
			fTestCompareLeftIsGreater(oStringCompare, tValue, svwzBlank);
			fTestCompareLeftIsEqual(oStringCompare, tValue, svwzNotBlank);
		};
		fTestCompre_w_NotBlank(pcwszNotBlank);
		fTestCompre_w_NotBlank(swNotBlank);
		fTestCompre_w_NotBlank(svwzNotBlank);
	};

	fTestCompare_BasicValueCombinations(oStringCompareCS);
	fTestCompare_BasicValueCombinations(oStringCompareCI);

	// Everything should be equal for case insensitive compare

	auto fTestCompare_a_CaseInsensitive = [&](const auto& tValue)
	{
		EXPECT_EQ(oStringCompareCI.Compare(tValue, pcaszLower), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, saLower), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, svazLower), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, pcaszUpper), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, saUpper), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, svazUpper), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, pcaszMixed), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, saMixed), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, svazMixed), 0);
	};
	fTestCompare_a_CaseInsensitive(pcaszLower);
	fTestCompare_a_CaseInsensitive(saLower);
	fTestCompare_a_CaseInsensitive(svazLower);
	fTestCompare_a_CaseInsensitive(pcaszUpper);
	fTestCompare_a_CaseInsensitive(saUpper);
	fTestCompare_a_CaseInsensitive(svazUpper);
	fTestCompare_a_CaseInsensitive(pcaszMixed);
	fTestCompare_a_CaseInsensitive(saMixed);
	fTestCompare_a_CaseInsensitive(svazMixed);

	auto fTestCompare_w_CaseInsensitive = [&](const auto& tValue)
	{
		EXPECT_EQ(oStringCompareCI.Compare(tValue, pcwszLower), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, swLower), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, svwzLower), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, pcwszUpper), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, swUpper), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, svwzUpper), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, pcwszMixed), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, swMixed), 0);
		EXPECT_EQ(oStringCompareCI.Compare(tValue, svwzMixed), 0);
	};
	fTestCompare_w_CaseInsensitive(pcwszLower);
	fTestCompare_w_CaseInsensitive(swLower);
	fTestCompare_w_CaseInsensitive(svwzLower);
	fTestCompare_w_CaseInsensitive(pcwszUpper);
	fTestCompare_w_CaseInsensitive(swUpper);
	fTestCompare_w_CaseInsensitive(svwzUpper);
	fTestCompare_w_CaseInsensitive(pcwszMixed);
	fTestCompare_w_CaseInsensitive(swMixed);
	fTestCompare_w_CaseInsensitive(svwzMixed);

	EXPECT_LT(oStringCompareCS.Compare("bello", "hello"), 0);
	EXPECT_LT(oStringCompareCI.Compare("bello", "hello"), 0);

	EXPECT_LT(oStringCompareCS.Compare("Hello", "hello"), 0);
	EXPECT_EQ(oStringCompareCI.Compare("Hello", "hello"), 0);

	// TODO: Much more...
}

TEST(StringCompare, Compare_AllTypeCombinations)
{
#define GEN_COMPARE_TO_LAMBDA(targetStringVar) \
	auto fTestCompareTo_##targetStringVar = [](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString) \
	{ \
		if constexpr (!std::is_same_v<decltype(tString), decltype(targetStringVar)>) \
		{ \
			EXPECT_TRUE(oStringCompare.AreEqual(tString, targetStringVar)); \
		} \
	}
	GEN_COMPARE_TO_LAMBDA(pcaszNotBlank);
	GEN_COMPARE_TO_LAMBDA(pcwszNotBlank);
	GEN_COMPARE_TO_LAMBDA(saNotBlank);
	GEN_COMPARE_TO_LAMBDA(swNotBlank);
	GEN_COMPARE_TO_LAMBDA(svazNotBlank);
	GEN_COMPARE_TO_LAMBDA(svwzNotBlank);
#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
	GEN_COMPARE_TO_LAMBDA(bsNotBlank);
#endif
#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
	GEN_COMPARE_TO_LAMBDA(bs2NotBlank);
#endif
#undef GEN_COMPARE_TO_LAMBDA

	auto fTestCompareTo_AllTypes = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		fTestCompareTo_pcaszNotBlank(oStringCompare, tString);
		fTestCompareTo_pcwszNotBlank(oStringCompare, tString);
		fTestCompareTo_saNotBlank(oStringCompare, tString);
		fTestCompareTo_swNotBlank(oStringCompare, tString);
		fTestCompareTo_svazNotBlank(oStringCompare, tString);
		fTestCompareTo_svwzNotBlank(oStringCompare, tString);
#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
		fTestCompareTo_bsNotBlank(oStringCompare, tString);
#endif
#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
		fTestCompareTo_bs2NotBlank(oStringCompare, tString);
#endif
	};

	auto fTestCompareWith_AllTypes = [&](const vlr::StringCompare::CComparator& oStringCompare)
	{
		fTestCompareTo_AllTypes(oStringCompare, pcaszNotBlank);
		fTestCompareTo_AllTypes(oStringCompare, pcwszNotBlank);
		fTestCompareTo_AllTypes(oStringCompare, saNotBlank);
		fTestCompareTo_AllTypes(oStringCompare, swNotBlank);
		fTestCompareTo_AllTypes(oStringCompare, svazNotBlank);
		fTestCompareTo_AllTypes(oStringCompare, svwzNotBlank);
#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
		fTestCompareTo_AllTypes(oStringCompare, bsNotBlank);
#endif
#if VLR_CONFIG_INCLUDE_ATL_CComBSTR
		fTestCompareTo_AllTypes(oStringCompare, bs2NotBlank);
#endif
	};

	fTestCompareWith_AllTypes(oStringCompareCS);
	fTestCompareWith_AllTypes(oStringCompareCI);
}

TEST(StringCompare, StringHasPrefix)
{
	auto fTestStringHasPrefix_AnythingHasBlankPrefix = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		EXPECT_EQ(oStringCompare.StringHasPrefix(tString, svazEmpty), true);
		EXPECT_EQ(oStringCompare.StringHasPrefix(tString, svwzEmpty), true);
	};
	auto fTestStringHasPrefix_LongerPrefixFalse = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		EXPECT_EQ(oStringCompare.StringHasPrefix(tString, "values"), false);
		EXPECT_EQ(oStringCompare.StringHasPrefix(tString, L"values"), false);
	};
	auto fTestStringHasPrefix_AnythingHasSelfPrefix = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		EXPECT_EQ(oStringCompare.StringHasPrefix(tString, tString), true);
	};
	auto fTestStringHasPrefix_BasicValueCombinations = [&](const vlr::StringCompare::CComparator& oStringCompare)
	{
		fTestStringHasPrefix_AnythingHasBlankPrefix(oStringCompare, svazEmpty);
		fTestStringHasPrefix_AnythingHasBlankPrefix(oStringCompare, svazBlank);
		fTestStringHasPrefix_AnythingHasBlankPrefix(oStringCompare, svazNotBlank);
		fTestStringHasPrefix_AnythingHasBlankPrefix(oStringCompare, svwzEmpty);
		fTestStringHasPrefix_AnythingHasBlankPrefix(oStringCompare, svwzBlank);
		fTestStringHasPrefix_AnythingHasBlankPrefix(oStringCompare, svwzNotBlank);

		fTestStringHasPrefix_LongerPrefixFalse(oStringCompare, svazNotBlank);
		fTestStringHasPrefix_LongerPrefixFalse(oStringCompare, svwzNotBlank);

		fTestStringHasPrefix_AnythingHasSelfPrefix(oStringCompare, svazNotBlank);
		fTestStringHasPrefix_AnythingHasSelfPrefix(oStringCompare, svwzNotBlank);
	};

	// CI

	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazLower, "va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazLower, "VA"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazLower, "Va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazLower, "vs"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazUpper, "va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazUpper, "VA"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazUpper, "Va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazMixed, "va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazMixed, "VA"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazMixed, "Va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazMixed, "vs"), false);

	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzLower, "va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazLower, "VA"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svazLower, "Va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzLower, "vs"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzUpper, "va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzUpper, "VA"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzUpper, "Va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzMixed, "va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzMixed, "VA"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzMixed, "Va"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPrefix(svwzMixed, "vs"), false);

	// CS

	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazLower, "va"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazLower, "VA"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazLower, "Va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazLower, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazUpper, "va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazUpper, "VA"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazUpper, "Va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazMixed, "va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazMixed, "VA"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazMixed, "Va"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svazMixed, "vs"), false);

	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzLower, "va"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzLower, "VA"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzLower, "Va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzLower, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzUpper, "va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzUpper, "VA"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzUpper, "Va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzMixed, "va"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzMixed, "VA"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzMixed, "Va"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPrefix(svwzMixed, "vs"), false);
}

TEST(StringCompare, StringHasPostfix)
{
	auto fTestStringHasPostfix_AnythingHasBlankPostfix = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		EXPECT_EQ(oStringCompare.StringHasPostfix(tString, svazEmpty), true);
		EXPECT_EQ(oStringCompare.StringHasPostfix(tString, svwzEmpty), true);
	};
	auto fTestStringHasPostfix_LongerPostfixFalse = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		EXPECT_EQ(oStringCompare.StringHasPostfix(tString, "values"), false);
		EXPECT_EQ(oStringCompare.StringHasPostfix(tString, "values"), false);
	};
	auto fTestStringHasPrefix_AnythingHasSelfPostfix = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		EXPECT_EQ(oStringCompare.StringHasPrefix(tString, tString), true);
	};
	auto fTestStringHasPostfix_BasicValueCombinations = [&](const vlr::StringCompare::CComparator& oStringCompare)
	{
		fTestStringHasPostfix_AnythingHasBlankPostfix(oStringCompare, svazEmpty);
		fTestStringHasPostfix_AnythingHasBlankPostfix(oStringCompare, svazBlank);
		fTestStringHasPostfix_AnythingHasBlankPostfix(oStringCompare, svazNotBlank);
		fTestStringHasPostfix_AnythingHasBlankPostfix(oStringCompare, svwzEmpty);
		fTestStringHasPostfix_AnythingHasBlankPostfix(oStringCompare, svwzBlank);
		fTestStringHasPostfix_AnythingHasBlankPostfix(oStringCompare, svwzNotBlank);

		fTestStringHasPostfix_LongerPostfixFalse(oStringCompare, svazNotBlank);
		fTestStringHasPostfix_LongerPostfixFalse(oStringCompare, svwzNotBlank);

		fTestStringHasPrefix_AnythingHasSelfPostfix(oStringCompare, svazNotBlank);
		fTestStringHasPrefix_AnythingHasSelfPostfix(oStringCompare, svwzNotBlank);
	};

	// CI

	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazLower, "ue"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazLower, "UE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazLower, "uE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazLower, "us"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazUpper, "ue"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazUpper, "UE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazUpper, "uE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazUpper, "us"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazMixed, "ue"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazMixed, "UE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazMixed, "uE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazMixed, "us"), false);

	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzLower, "ue"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazLower, "UE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svazLower, "uE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzLower, "vs"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzUpper, "ue"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzUpper, "UE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzUpper, "uE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzMixed, "ue"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzMixed, "UE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzMixed, "uE"), true);
	EXPECT_EQ(oStringCompareCI.StringHasPostfix(svwzMixed, "vs"), false);

	// CS

	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazLower, "ue"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazLower, "UE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazLower, "uE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazLower, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazUpper, "ue"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazUpper, "UE"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazUpper, "uE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazMixed, "ue"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazMixed, "UE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazMixed, "uE"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svazMixed, "vs"), false);

	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzLower, "ue"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzLower, "UE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzLower, "uE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzLower, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzUpper, "ue"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzUpper, "UE"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzUpper, "uE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzUpper, "vs"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzMixed, "ue"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzMixed, "UE"), false);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzMixed, "uE"), true);
	EXPECT_EQ(oStringCompareCS.StringHasPostfix(svwzMixed, "vs"), false);
}

TEST(StringCompare, StringFindIndexOfSubstring)
{
	auto fTestStringHasSubstring_AnyStringHasBlankSubstring = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString)
	{
		size_t nExpectedIndex = 0;
		EXPECT_EQ(oStringCompare.StringFindIndexOfSubstring(tString, svazEmpty), nExpectedIndex);
		EXPECT_EQ(oStringCompare.StringFindIndexOfSubstring(tString, svwzEmpty), nExpectedIndex);
	};

	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, svazEmpty);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, svazBlank);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, svazNotBlank);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, svwzEmpty);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, svwzBlank);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, svwzNotBlank);

	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, svazEmpty);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, svazBlank);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, svazNotBlank);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, svwzEmpty);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, svwzBlank);
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, svwzNotBlank);

	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, "test");
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCS, L"test");

	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, "test");
	fTestStringHasSubstring_AnyStringHasBlankSubstring(oStringCompareCI, L"test");

	auto fTestStringHasSubstring_Valid = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString, const auto& tSubstring, size_t nExpectedIndex)
	{
		EXPECT_EQ(oStringCompare.StringFindIndexOfSubstring(tString, tSubstring), nExpectedIndex);
	};

	fTestStringHasSubstring_Valid(oStringCompareCS, "testme", "me", 4);
	fTestStringHasSubstring_Valid(oStringCompareCS, L"testme", L"me", 4);
	fTestStringHasSubstring_Valid(oStringCompareCI, "testme", "me", 4);
	fTestStringHasSubstring_Valid(oStringCompareCI, L"testme", L"me", 4);

	auto fTestStringHasSubstring_Invalid = [&](const vlr::StringCompare::CComparator& oStringCompare, const auto& tString, const auto& tSubstring)
	{
		size_t nExpectedIndex = std::string_view::npos;
		EXPECT_EQ(oStringCompare.StringFindIndexOfSubstring(tString, tSubstring), nExpectedIndex);
	};

	fTestStringHasSubstring_Invalid(oStringCompareCS, "testme", "you");
	fTestStringHasSubstring_Invalid(oStringCompareCS, L"testme", L"you");
	fTestStringHasSubstring_Invalid(oStringCompareCI, "testme", "you");
	fTestStringHasSubstring_Invalid(oStringCompareCI, L"testme", L"you");
}
