#include "pch.h"

#include "vlr/util.data_adaptor.MultiSZ.h"

TEST( util_data_adaptor_MultiSZ, general )
{
	using namespace vlr::util;

	constexpr auto psazTest1 = "foo\0";
	constexpr auto psazTest2 = "foo\0bar\0";
	constexpr auto pswzTest1 = L"foo\0";
	constexpr auto pswzTest2 = L"foo\0bar\0";

	{
		auto oValueList = data_adaptor::MultiSZToStructuredData( psazTest1 );
		EXPECT_EQ( oValueList.size(), 1 );
		EXPECT_STREQ( oValueList.front().c_str(), "foo" );
	}
	{
		auto oValueList = data_adaptor::MultiSZToStructuredData( psazTest2 );
		EXPECT_EQ( oValueList.size(), 2 );
		EXPECT_STREQ( oValueList.front().c_str(), "foo" );
		EXPECT_STREQ( oValueList.back().c_str(), "bar" );
	}
	{
		auto oValueList = data_adaptor::MultiSZToStructuredData( pswzTest1 );
		EXPECT_EQ( oValueList.size(), 1 );
		EXPECT_STREQ( oValueList.front().c_str(), L"foo" );
	}
	{
		auto oValueList = data_adaptor::MultiSZToStructuredData( pswzTest2 );
		EXPECT_EQ( oValueList.size(), 2 );
		EXPECT_STREQ( oValueList.front().c_str(), L"foo" );
		EXPECT_STREQ( oValueList.back().c_str(), L"bar" );
	}
}
