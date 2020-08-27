#include "pch.h"
#include <tchar.h>

#include "../vlr-util/util_IsNotBlank.h"

TEST( util_IsNotBlank, general )
{
	EXPECT_EQ( vlr::util::IsNotBlank( std::string_view{ "" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( std::string_view{ "foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( std::wstring_view{ L"" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( std::wstring_view{ L"foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( "" ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( "foo" ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( L"" ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( L"foo" ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( vlr::zstring_view{ "" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( vlr::zstring_view{ "foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( vlr::wzstring_view{ L"" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( vlr::wzstring_view{ L"foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( std::string{} ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( std::string{ "" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( std::string{ "foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( std::wstring{} ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( std::wstring{ L"" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( std::wstring{ L"foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( CString{} ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( CString{ _T( "" ) } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( CString{ _T( "foo" ) } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( CStringA{} ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( CStringA{ "" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( CStringA{ "foo" } ), true );
	EXPECT_EQ( vlr::util::IsNotBlank( CStringW{} ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( CStringW{ L"" } ), false );
	EXPECT_EQ( vlr::util::IsNotBlank( CStringW{ L"foo" } ), true );
}
