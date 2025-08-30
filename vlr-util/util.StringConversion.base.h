#pragma once

#include <string>
#include <string_view>

#include "config.h"
#include "UtilMacros.CompilerSupport.h"
#include "zstring_view.h"

namespace vlr {

namespace util {

namespace CodePageWin32 {

// Note: This must match defines in WinNls.h; code from there for reference:
//
//  Code Page Default Values.
//  Please Use Unicode, either UTF-16 (as in WCHAR) or UTF-8 (code page CP_ACP)
//
//#define CP_ACP                    0           // default to ANSI code page
//#define CP_OEMCP                  1           // default to OEM  code page
//#define CP_MACCP                  2           // default to MAC  code page
//#define CP_THREAD_ACP             3           // current thread's ANSI code page
//#define CP_SYMBOL                 42          // SYMBOL translations
//
//#define CP_UTF7                   65000       // UTF-7 translation
//#define CP_UTF8                   65001       // UTF-8 translation

enum ECodePageWin32
{
	ANSI = 0,
	OEM = 1,
	MAC = 2,
	ANSI_CurrentThread = 3,
	SYMBOL = 42,
	UTF7 = 65000,
	UTF8 = 65001,
};

} // namespace CodePageWin32

struct StringConversionResults;

// Note: We want StringConversionOptions to be as constexpr constructable (for default case) as possible,
// so that the compiler can inline and optimize that case as much as possible.

struct StringConversionOptions
{
public:
	bool m_bInputStringIsNullTerminated = false;
	bool m_bGenerateResultNotNullTerminated = false;
	UINT m_nCodePage = CodePageWin32::UTF8;
	DWORD m_dwFlags_MultiByteToWideChar = 0;
	DWORD m_dwFlags_WideCharToMultiByte = 0;

public:
	constexpr auto& withNullTerminatedString(bool bValue = true)
	{
		m_bInputStringIsNullTerminated = bValue;
		return *this;
	}
	constexpr auto& withGenerateResultNotNullTerminated(bool bValue = true)
	{
		m_bGenerateResultNotNullTerminated = bValue;
		return *this;
	}
	constexpr auto& withCodePage(UINT nCodePage)
	{
		m_nCodePage = nCodePage;
		return *this;
	}
	// Note: This is arbitrary and can change between systems. Try not to use.
	// See: https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
	constexpr auto& withCodePage_SystemDefaultASCII()
	{
		m_nCodePage = CodePageWin32::ANSI;
		return *this;
	}
	constexpr auto& withCodePage_UTF8()
	{
		m_nCodePage = CodePageWin32::UTF8;
		return *this;
	}
	constexpr auto& withFlags_MultiByteToWideChar(DWORD dwFlags_MultiByteToWideChar)
	{
		m_dwFlags_MultiByteToWideChar = dwFlags_MultiByteToWideChar;
		return *this;
	}
	constexpr auto& withFlags_WideCharToMultiByte(DWORD dwFlags_WideCharToMultiByte)
	{
		m_dwFlags_WideCharToMultiByte = dwFlags_WideCharToMultiByte;
		return *this;
	}

public:
	constexpr UINT GetCodePageIdentifier() const
	{
		return m_nCodePage;
	}
	constexpr DWORD OnMultiByteToWideChar_GetFlags() const
	{
		return m_dwFlags_MultiByteToWideChar;
	}
	constexpr DWORD OnWideCharToMultiByte_GetFlags() const
	{
		return m_dwFlags_WideCharToMultiByte;
	}
	constexpr const CHAR* OnWideCharToMultiByte_GetDefaultChar() const
	{
		// Note: If codepage if UTF7/UTF8, this must be NULL
		return nullptr;
	}
	constexpr WIN_BOOL* OnWideCharToMultiByte_GetUsedDefaultChar(StringConversionResults* /*pStringConversionResults*/) const
	{
		// Note: If codepage if UTF7/UTF8, this must be NULL
		return nullptr;
	}

public:
	static constexpr auto ForSystemDefaultASCIIIngress()
	{
		StringConversionOptions theInstance;
		theInstance.m_nCodePage = CodePageWin32::ANSI;
		return theInstance;
	}

	VLR_DEFINE_DEFAULT_CONSTRUCTOR_CONSTEXPR_NOEXCEPT(StringConversionOptions);
};

struct StringConversionResults
{
public:
	size_t m_nOuputSizeBytes = 0;
	std::string m_saResultOnException;
	std::wstring m_swResultOnException;
};

} // namespace util

} // namespace vlr
