#pragma once

#if !defined(WIN32)
#error This file should only be included while compiling for Windows
#endif

#include "config.h"

#include <cstringt.h>

#include "util.StringConversion.base.h"

namespace vlr {

namespace util {

namespace win32 {

HRESULT MultiByte_to_UTF16(
	std::string_view svValue,
	wchar_t* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions,
	StringConversionResults* pStringConversionResults);
HRESULT UTF16_to_MultiByte(
	std::wstring_view svValue,
	char* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions,
	StringConversionResults* pStringConversionResults);

HRESULT MultiByte_to_UTF16(
	std::string_view svValue,
	CStringW& sOutput,
	const StringConversionOptions& oStringConversionOptions = {},
	StringConversionResults* pStringConversionResults = nullptr);
HRESULT UTF16_to_MultiByte(
	std::wstring_view svValue,
	CStringA& sOutput,
	const StringConversionOptions& oStringConversionOptions = {},
	StringConversionResults* pStringConversionResults = nullptr);

} // namespace win32

} // namespace util

} // namespace vlr
