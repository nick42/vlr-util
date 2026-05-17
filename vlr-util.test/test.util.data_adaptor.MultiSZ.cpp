#include "pch.h"

#include "vlr-util/util.data_adaptor.MultiSZ.h"

using namespace vlr::util::data_adaptor;

// Note: The following tests are for the deprecated API, so we disable the deprecation warning for this section
#pragma warning(push)
#pragma warning(disable: 4996) // 'function': was declared deprecated

TEST( util_data_adaptor_MultiSZ, general )
{
	using namespace vlr::util;

	constexpr auto psazTest1 = "foo\0";
	constexpr auto psazTest2 = "foo\0bar\0";
	constexpr auto pswzTest1 = L"foo\0";
	constexpr auto pswzTest2 = L"foo\0bar\0";

	{
		auto oValueList = MultiSZToStructuredData( psazTest1 );
		EXPECT_EQ( oValueList.size(), 1 );
		EXPECT_STREQ( oValueList.front().c_str(), "foo" );
	}
	{
		auto oValueList = MultiSZToStructuredData( psazTest2 );
		EXPECT_EQ( oValueList.size(), 2 );
		EXPECT_STREQ( oValueList.front().c_str(), "foo" );
		EXPECT_STREQ( oValueList.back().c_str(), "bar" );
	}
	{
		auto oValueList = MultiSZToStructuredData( pswzTest1 );
		EXPECT_EQ( oValueList.size(), 1 );
		EXPECT_STREQ( oValueList.front().c_str(), L"foo" );
	}
	{
		auto oValueList = MultiSZToStructuredData( pswzTest2 );
		EXPECT_EQ( oValueList.size(), 2 );
		EXPECT_STREQ( oValueList.front().c_str(), L"foo" );
		EXPECT_STREQ( oValueList.back().c_str(), L"bar" );
	}
}

#pragma warning(pop)

class MultiSZToStructuredDataTests : public ::testing::Test
{
};

TEST_F(MultiSZToStructuredDataTests, ValidSingleString)
{
	const wchar_t input[] = L"hello\0\0";
	cpp::span<const wchar_t> span(input, 8);
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(S_OK, hr);
	EXPECT_EQ(1, result.size());
	EXPECT_EQ(5, result[0].length());
}

TEST_F(MultiSZToStructuredDataTests, ValidMultipleStrings)
{
	const wchar_t input[] = L"first\0second\0third\0\0";
	cpp::span<const wchar_t> span(input, 23);
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(S_OK, hr);
	EXPECT_EQ(3, result.size());
	EXPECT_EQ(5, result[0].length());
	EXPECT_EQ(6, result[1].length());
	EXPECT_EQ(5, result[2].length());
}

TEST_F(MultiSZToStructuredDataTests, EmptyMultiSZ)
{
	const wchar_t input[] = L"\0\0";
	cpp::span<const wchar_t> span(input, 2);
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(S_OK, hr);
	EXPECT_EQ(0, result.size());
}

TEST_F(MultiSZToStructuredDataTests, EmptySpan)
{
	cpp::span<const wchar_t> span;
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(E_FAIL, hr);
}

TEST_F(MultiSZToStructuredDataTests, MissingDoubleNull)
{
	const wchar_t input[] = L"hello\0world";
	cpp::span<const wchar_t> span(input, 11);
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(E_FAIL, hr);
}

TEST_F(MultiSZToStructuredDataTests, SingleNullTerminator)
{
	const wchar_t input[] = L"\0";
	cpp::span<const wchar_t> span(input, 1);
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(E_FAIL, hr);
}

TEST_F(MultiSZToStructuredDataTests, EmptyStringInMiddle)
{
	const wchar_t input[] = L"first\0\0second\0\0";
	cpp::span<const wchar_t> span(input, 15);
	std::vector<vlr::basic_zstring_view<wchar_t>> result;

	HRESULT hr = HelperFor_MultiSZ<wchar_t>{}.ToStructuredData(span, result);

	EXPECT_EQ(S_OK, hr);
	EXPECT_EQ(1, result.size());
	EXPECT_EQ(5, result[0].length());
}

TEST_F(MultiSZToStructuredDataTests, AnsiCharacters)
{
	const char input[] = "hello\0world\0\0";
	cpp::span<const char> span(input, 14);
	std::vector<vlr::basic_zstring_view<char>> result;

	HRESULT hr = HelperFor_MultiSZ<char>{}.ToStructuredData(span, result);

	EXPECT_EQ(S_OK, hr);
	EXPECT_EQ(2, result.size());
	EXPECT_EQ(5, result[0].length());
	EXPECT_EQ(5, result[1].length());
}
