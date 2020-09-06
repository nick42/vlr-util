#pragma once

#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.range_checked_cast.h"
#include "util.Unicode.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( Convert )

// std::string <- std::string_view

inline decltype(auto) ToStdStringA( std::string_view svValue )
{
	return std::string{ svValue };
}

inline decltype(auto) ToStdStringA( std::wstring_view svValue )
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_StdString( svValue );
}

inline decltype(auto) ToStdStringW( std::string_view svValue )
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_StdString( svValue );
}

inline decltype(auto) ToStdStringW( std::wstring_view svValue )
{
	return std::wstring{ svValue };
}

// std::string <- std::string

inline decltype(auto) ToStdStringA( const std::string& sValue )
{
	return sValue;
}

inline decltype(auto) ToStdStringA( const std::wstring& sValue )
{
	auto svValue = std::wstring_view{ sValue };
	return ToStdStringA( svValue );
}

inline decltype(auto) ToStdStringW( const std::string& sValue )
{
	auto svValue = std::string_view{ sValue };
	return ToStdStringW( svValue );
}

inline decltype(auto) ToStdStringW( const std::wstring& sValue )
{
	return sValue;
}

// std::string <- CString

inline decltype(auto) ToStdStringA( const CStringA& saValue )
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return std::string{ svValue };
}

inline decltype(auto) ToStdStringA( const CStringW& swValue )
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return ToStdStringA( svValue );
}

inline decltype(auto) ToStdStringW( const CStringA& saValue )
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return ToStdStringW( svValue );
}

inline decltype(auto) ToStdStringW( const CStringW& swValue )
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return std::wstring{ svValue };
}

// Generic version for "StdStringT", based on compilation type

template< typename TString >
inline decltype(auto) ToStdString( const TString& tString )
{
	if constexpr (sizeof( TCHAR ) == sizeof( char ))
	{
		return ToStdStringA( tString );
	}
	else if constexpr (sizeof( TCHAR ) == sizeof( wchar_t ))
	{
		return ToStdStringW( tString );
	}
	else
	{
		static_assert(false, "Unhandled character size");
	}
}

// std::string <- std::string_view

inline decltype(auto) ToCStringA( std::string_view svValue )
{
	return CStringA{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

inline decltype(auto) ToCStringA( std::wstring_view svValue )
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte_CString( svValue );
}

inline decltype(auto) ToCStringW( std::string_view svValue )
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16_CString( svValue );
}

inline decltype(auto) ToCStringW( std::wstring_view svValue )
{
	return CStringW{ svValue.data(), range_checked_cast<int>(svValue.length()) };
}

// CString <- CString

inline decltype(auto) ToCStringA( const CStringA& sValue )
{
	return sValue;
}

inline decltype(auto) ToCStringA( const CStringW& sValue )
{
	auto svValue = std::wstring_view{ sValue.GetString(), util::range_checked_cast<size_t>(sValue.GetLength()) };
	return ToCStringA( svValue );
}

inline decltype(auto) ToCStringW( const CStringA& sValue )
{
	auto svValue = std::string_view{ sValue.GetString(), util::range_checked_cast<size_t>(sValue.GetLength()) };
	return ToCStringW( svValue );
}

inline decltype(auto) ToCStringW( const CStringW& sValue )
{
	return sValue;
}

// CString <- std::string

inline decltype(auto) ToCStringA( const std::string& sValue )
{
	auto svValue = std::string_view{ sValue };
	return ToCStringA( svValue );
}

inline decltype(auto) ToCStringA( const std::wstring& sValue )
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringA( svValue );
}

inline decltype(auto) ToCStringW( const std::string& sValue )
{
	auto svValue = std::string_view{ sValue };
	return ToCStringW( svValue );
}

inline decltype(auto) ToCStringW( const std::wstring& sValue )
{
	auto svValue = std::wstring_view{ sValue };
	return ToCStringW( svValue );
}

// Generic version for "CStringT", based on compilation type

template< typename TString >
inline decltype(auto) ToCString( const TString& tString )
{
	if constexpr (sizeof( TCHAR ) == sizeof( char ))
	{
		return ToCStringA( tString );
	}
	else if constexpr (sizeof( TCHAR ) == sizeof( wchar_t ))
	{
		return ToCStringW( tString );
	}
	else
	{
		static_assert(false, "Unhandled character size");
	}
}

NAMESPACE_END //( Convert )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
