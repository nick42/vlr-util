#include "pch.h"

#include "vlr-util/as_span.h"

TEST( as_span, general )
{
	auto oVectorBYTE = std::vector<BYTE>{ 1, 2 };
	{
		auto oSpan = vlr::as_span<BYTE>{ oVectorBYTE };
		EXPECT_EQ( (void*)oSpan.data(), (void*)oVectorBYTE.data() );
		EXPECT_EQ( oSpan.size(), oVectorBYTE.size() );
	}
	{
		auto oSpan = vlr::as_span<const BYTE>{ oVectorBYTE };
		EXPECT_EQ( (void*)oSpan.data(), (void*)oVectorBYTE.data() );
		EXPECT_EQ( oSpan.size(), oVectorBYTE.size() );
	}

	auto strData = std::string{ "test" };
	{
		auto oSpan = vlr::as_span<char>{ strData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strData.data() );
		EXPECT_EQ( oSpan.size(), strData.size() );
	}
	{
		auto oSpan = vlr::as_span<const char>{ strData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strData.data() );
		EXPECT_EQ( oSpan.size(), strData.size() );
	}
	// Treating char_t array as span of BYTEs
	{
		auto oSpan = vlr::as_span<BYTE>{ strData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strData.data() );
		EXPECT_EQ( oSpan.size(), strData.size() );
	}
	{
		auto oSpan = vlr::as_span<const BYTE>{ strData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strData.data() );
		EXPECT_EQ( oSpan.size(), strData.size() );
	}

	auto strwData = std::wstring{ L"test" };
	{
		auto oSpan = vlr::as_span<wchar_t>{ strwData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strwData.data() );
		EXPECT_EQ( oSpan.size(), strwData.size() );
	}
	{
		auto oSpan = vlr::as_span<const wchar_t>{ strwData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strwData.data() );
		EXPECT_EQ( oSpan.size(), strwData.size() );
	}
	// Treating wchar_t array as span of BYTEs
	// (note the size multiplier for the check)
	{
		auto oSpan = vlr::as_span<BYTE>{ strwData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strwData.data() );
		EXPECT_EQ( oSpan.size(), strwData.size() * sizeof( wchar_t ) );
	}
	{
		auto oSpan = vlr::as_span<const BYTE>{ strwData };
		EXPECT_EQ( (void*)oSpan.data(), (void*)strwData.data() );
		EXPECT_EQ( oSpan.size(), strwData.size() * sizeof( wchar_t ) );
	}
}
