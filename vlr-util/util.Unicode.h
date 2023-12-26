#pragma once

#include <string>
#include <string_view>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "zstring_view.h"

namespace vlr {

namespace util {

struct StringConversionResults;

struct StringConversionOptions
{
public:
	bool m_bInputStringIsNullTerminated = false;
	bool m_bGenerateResultNotNullTerminated = false;

public:
	inline auto& WithNullTerminatedString( bool bValue = true )
	{
		m_bInputStringIsNullTerminated = bValue;
		return *this;
	}
	inline auto& With_GenerateResultNotNullTerminated( bool bValue = true )
	{
		m_bGenerateResultNotNullTerminated = bValue;
		return *this;
	}

public:
	UINT GetCodePageIdentifier() const
	{
		return CP_UTF8;
	}
	DWORD OnMultiByteToWideChar_GetFlags() const
	{
		return 0;
	}
	DWORD OnWideCharToMultiByte_GetFlags() const
	{
		return 0;
	}
	const CHAR* OnWideCharToMultiByte_GetDefaultChar() const
	{
		// Note: If codepage if UTF7/UTF8, this must be NULL
		return nullptr;
	}
	BOOL* OnWideCharToMultiByte_GetUsedDefaultChar( StringConversionResults* /*pStringConversionResults*/ ) const
	{
		// Note: If codepage if UTF7/UTF8, this must be NULL
		return nullptr;
	}
};

struct StringConversionResults
{
public:
	size_t m_nOuputSizeBytes = 0;
};

class CStringConversion
{
public:
	HRESULT MultiByte_to_UTF16(
		std::string_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );
	HRESULT UTF16_to_MultiByte(
		std::wstring_view svValue,
		char* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );

	HRESULT MultiByte_to_UTF16(
		vlr::zstring_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );
	HRESULT UTF16_to_MultiByte(
		vlr::wzstring_view svValue,
		char* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );

	HRESULT MultiByte_to_UTF16(
		std::string_view svValue,
		std::wstring& strOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );
	HRESULT UTF16_to_MultiByte(
		std::wstring_view svValue,
		std::string& strOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );

	HRESULT MultiByte_to_UTF16(
		const std::string& strValue,
		std::wstring& strOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );
	HRESULT UTF16_to_MultiByte(
		const std::wstring& strValue,
		std::string& strOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );

	auto Inline_MultiByte_to_UTF16_StdString(
		std::string_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		std::wstring strOutput;
		MultiByte_to_UTF16( svValue, strOutput, oStringConversionOptions, pStringConversionResults );
		return strOutput;
	}
	auto Inline_UTF16_to_MultiByte_StdString(
		std::wstring_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		std::string strOutput;
		UTF16_to_MultiByte( svValue, strOutput, oStringConversionOptions, pStringConversionResults );
		return strOutput;
	}

	auto Inline_MultiByte_to_UTF16_StdString(
		const std::string& strValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		std::wstring strOutput;
		MultiByte_to_UTF16( strValue, strOutput, oStringConversionOptions, pStringConversionResults );
		return strOutput;
	}
	auto Inline_UTF16_to_MultiByte_StdString(
		const std::wstring& strValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		std::string strOutput;
		UTF16_to_MultiByte( strValue, strOutput, oStringConversionOptions, pStringConversionResults );
		return strOutput;
	}

#if VLR_CONFIG_INCLUDE_ATL_CSTRING
	HRESULT MultiByte_to_UTF16(
		std::string_view svValue,
		CStringW& sOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );
	HRESULT UTF16_to_MultiByte(
		std::wstring_view svValue,
		CStringA& sOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr );

	auto Inline_MultiByte_to_UTF16_CString(
		std::string_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		CStringW sOutput;
		MultiByte_to_UTF16( svValue, sOutput, oStringConversionOptions, pStringConversionResults );
		return sOutput;
	}
	auto Inline_UTF16_to_MultiByte_CString(
		std::wstring_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		CStringA sOutput;
		UTF16_to_MultiByte( svValue, sOutput, oStringConversionOptions, pStringConversionResults );
		return sOutput;
	}
#endif // VLR_CONFIG_INCLUDE_ATL_CSTRING

};

} // namespace util

} // namespace vlr
