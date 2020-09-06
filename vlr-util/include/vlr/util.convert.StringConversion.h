#pragma once

#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.Unicode.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( Convert )

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

inline auto ToStdStringW( const std::wstring& strValue )
-> std::wstring
{
	return strValue;
}

inline auto ToStdStringW( const std::string& strValue )
-> std::wstring
{
	return CStringConversion{}.Inline_MultiByte_to_UTF16( strValue );
}

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
