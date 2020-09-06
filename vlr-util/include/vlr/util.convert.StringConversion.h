#pragma once

#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.range_checked_cast.h"
#include "util.Unicode.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( Convert )

// std::string <- std::string

inline auto ToStdStringA( const std::string& strValue )
-> std::string
{
	return strValue;
}

inline auto ToStdStringA( const std::wstring& strValue )
-> std::string
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte( strValue );
}

inline auto ToStdStringW( const std::string& strValue )
-> std::wstring
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16( strValue );
}

inline auto ToStdStringW( const std::wstring& strValue )
-> std::wstring
{
	return strValue;
}

// std::string <- std::string_view

inline auto ToStdStringA( std::string_view svValue )
-> std::string
{
	return std::string{ svValue };
}

inline auto ToStdStringA( std::wstring_view svValue )
-> std::string
{
	return CStringConversion{}.Inline_UTF16_to_MultiByte( svValue );
}

inline auto ToStdStringW( std::string_view svValue )
-> std::wstring
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16( svValue );
}

inline auto ToStdStringW( std::wstring_view svValue )
-> std::wstring
{
	return std::wstring{ svValue };
}

// std::string <- CString

inline auto ToStdStringA( const CStringA& saValue )
-> std::string
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return std::string{ svValue };
}

inline auto ToStdStringA( const CStringW& swValue )
-> std::string
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return ToStdStringA( svValue );
}

inline auto ToStdStringW( const CStringA& saValue )
-> std::wstring
{
	auto svValue = std::string_view{ saValue.GetString(), util::range_checked_cast<size_t>(saValue.GetLength()) };
	return ToStdStringW( svValue );
}

inline auto ToStdStringW( const CStringW& swValue )
-> std::wstring
{
	auto svValue = std::wstring_view{ swValue.GetString(), util::range_checked_cast<size_t>(swValue.GetLength()) };
	return std::wstring{ svValue };
}

// Generic version for "StdStringT", based on compilation type

template< typename TString >
inline auto ToStdString( const TString& tString )
{
	if constexpr (sizeof( TCHAR ) == sizeof( WCHAR ))
	{
		return ToStdStringW( tString );
	}
	else
	{
		return ToStdStringA( tString );
	}
}

NAMESPACE_END //( Convert )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
