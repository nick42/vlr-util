#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <string_view>

#include "config.h"
#include "util.StringConversion.base.h"
#include "util.choice.h"
#include "UtilMacros.Assertions.h"
#include "UtilMacros.General.h"

#if defined(WIN32)
#include "Win32/util.win32.StringConversion.h"
#endif

// For dev testing (should be commented out in published version):
//#define VLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS

namespace vlr {

namespace util {

namespace StringConversion {

class CExternalImpl
{
public:
	static CExternalImpl& GetSharedInstanceMutable();
	static const auto& GetSharedInstance()
	{
		return GetSharedInstanceMutable();
	}

public:
	using FMultiByte_to_UTF16_StdString_Inline = std::function<std::wstring(std::string_view svValue, const StringConversionOptions& oStringConversionOptions)>;
	using FUTF16_to_MultiByte_StdString_Inline = std::function<std::string(std::wstring_view svValue, const StringConversionOptions& oStringConversionOptions)>;

protected:
	mutable std::mutex m_mutexDataAccess;
	FMultiByte_to_UTF16_StdString_Inline m_fMultiByte_to_UTF16_StdString_Inline;
	FUTF16_to_MultiByte_StdString_Inline m_fUTF16_to_MultiByte_StdString_Inline;

public:
	// Note: Returns Success_NoWorkDone if there is no function set

	inline SResult Call_MultiByte_to_UTF16_StdString(
		std::string_view svValue,
		std::wstring& swValue,
		const StringConversionOptions& oStringConversionOptions) const
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		if (!m_fMultiByte_to_UTF16_StdString_Inline)
		{
			return SResult::Success_NoWorkDone;
		}

		swValue = m_fMultiByte_to_UTF16_StdString_Inline(svValue, oStringConversionOptions);

		return SResult::Success;
	}
	inline SResult Call_UTF16_to_MultiByte_StdString(
		std::wstring_view svValue,
		std::string& saValue,
		const StringConversionOptions& oStringConversionOptions) const
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		if (!m_fUTF16_to_MultiByte_StdString_Inline)
		{
			return SResult::Success_NoWorkDone;
		}

		saValue = m_fUTF16_to_MultiByte_StdString_Inline(svValue, oStringConversionOptions);

		return SResult::Success;
	}

	inline SResult Set_MultiByte_to_UTF16_StdString_Inline(const FMultiByte_to_UTF16_StdString_Inline& fMultiByte_to_UTF16_StdString_Inline)
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		m_fMultiByte_to_UTF16_StdString_Inline = fMultiByte_to_UTF16_StdString_Inline;

		return SResult::Success;
	}
	inline SResult Set_MultiByte_to_UTF16_StdString_Inline(FMultiByte_to_UTF16_StdString_Inline&& fMultiByte_to_UTF16_StdString_Inline)
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		m_fMultiByte_to_UTF16_StdString_Inline = std::move(fMultiByte_to_UTF16_StdString_Inline);

		return SResult::Success;
	}
	inline SResult Set_UTF16_to_MultiByte_StdString_Inline(const FUTF16_to_MultiByte_StdString_Inline& fUTF16_to_MultiByte_StdString_Inline)
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		m_fUTF16_to_MultiByte_StdString_Inline = fUTF16_to_MultiByte_StdString_Inline;

		return SResult::Success;
	}
	inline SResult Set_UTF16_to_MultiByte_StdString_Inline(FUTF16_to_MultiByte_StdString_Inline&& fUTF16_to_MultiByte_StdString_Inline)
	{
		auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

		m_fUTF16_to_MultiByte_StdString_Inline = std::move(fUTF16_to_MultiByte_StdString_Inline);

		return SResult::Success;
	}

};

} // namespace StringConversion

class CStringConversion
{
protected:
	// choice<0> is the externally defined inline fallback. If this is defined, call by preference.

#if defined(VLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS)
	inline HRESULT MultiByte_to_UTF16_choice(
		vlr::util::choice<0>&&,
		std::string_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults)
	{
		SResult sr;

		std::wstring swValue;
		sr = StringConversion::CExternalImpl::GetSharedInstance().Call_MultiByte_to_UTF16_StdString(svValue, swValue, oStringConversionOptions);
		if (sr != SResult::Success)
		{
			// Call the next potential conversion explicitly
			return MultiByte_to_UTF16_choice(
				vlr::util::choice<1>{},
				svValue,
				pOutputBuffer,
				nOutputBufferLengthBytes,
				oStringConversionOptions,
				pStringConversionResults);
		}

		if (nOutputBufferLengthBytes < swValue.length() + sizeof(wchar_t))
		{
			return E_FAIL;
		}

#if defined(WIN32)
		wcscpy_s(pOutputBuffer, nOutputBufferLengthBytes / sizeof(wchar_t), swValue.c_str());
#else
		wcscpy(pOutputBuffer, swValue.c_str());
#endif

		VLR_IF_NOT_NULL_DEREF(pStringConversionResults).m_nOuputSizeBytes = swValue.length() * sizeof(wchar_t);

		return S_OK;
	}
	inline HRESULT UTF16_to_MultiByte_choice(
		vlr::util::choice<0>&&,
		std::wstring_view svValue,
		char* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults)
	{
		SResult sr;

		std::string saValue;
		sr = StringConversion::CExternalImpl::GetSharedInstance().Call_UTF16_to_MultiByte_StdString(svValue, saValue, oStringConversionOptions);
		if (sr != SResult::Success)
		{
			// Call the next potential conversion explicitly
			return UTF16_to_MultiByte_choice(
				vlr::util::choice<1>{},
				svValue,
				pOutputBuffer,
				nOutputBufferLengthBytes,
				oStringConversionOptions,
				pStringConversionResults);
		}

		if (nOutputBufferLengthBytes < saValue.length() + sizeof(char))
		{
			return E_FAIL;
		}

#if defined(WIN32)
		strcpy_s(pOutputBuffer, nOutputBufferLengthBytes / sizeof(char), saValue.c_str());
#else
		strcpy(pOutputBuffer, saValue.c_str());
#endif

		VLR_IF_NOT_NULL_DEREF(pStringConversionResults).m_nOuputSizeBytes = saValue.length() * sizeof(char);

		return S_OK;
	}
#endif // defined(VLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS)

	// Note: choice<1> through choice<9> are internally defined platform specific methods (with room to expand).

#if defined(WIN32)
	inline HRESULT MultiByte_to_UTF16_choice(
		vlr::util::choice<1>&&,
		std::string_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults)
	{
		return win32::MultiByte_to_UTF16(svValue, pOutputBuffer, nOutputBufferLengthBytes, oStringConversionOptions, pStringConversionResults);
	}
	inline HRESULT UTF16_to_MultiByte_choice(
		vlr::util::choice<1>&&,
		std::wstring_view svValue,
		char* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults)
	{
		return win32::UTF16_to_MultiByte(svValue, pOutputBuffer, nOutputBufferLengthBytes, oStringConversionOptions, pStringConversionResults);
	}
#endif // defined(WIN32)

	// choice<10> would be the default, but there is no current default (not possible in non-deprecated std code).
	// Note: Potentially enhance this in the future based on an optional dependency on the utf external library.
	// Note: This must be compiled in now, based on updated library design.
	 
	inline HRESULT MultiByte_to_UTF16_choice(
		vlr::util::choice<10>&&,
		std::string_view /*svValue*/,
		wchar_t* /*pOutputBuffer*/,
		size_t /*nOutputBufferLengthBytes*/,
		const StringConversionOptions& /*oStringConversionOptions*/,
		StringConversionResults* /*pStringConversionResults*/)
	{
		return E_FAIL;
	}
	inline HRESULT UTF16_to_MultiByte_choice(
		vlr::util::choice<10>&&,
		std::wstring_view /*svValue*/,
		char* /*pOutputBuffer*/,
		size_t /*nOutputBufferLengthBytes*/,
		const StringConversionOptions& /*oStringConversionOptions*/,
		StringConversionResults* /*pStringConversionResults*/)
	{
		return E_FAIL;
	}

public:
	inline HRESULT MultiByte_to_UTF16(
		std::string_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr)
	{
		if (svValue.empty())
		{
			VLR_IF_NOT_NULL_DEREF(pStringConversionResults).m_nOuputSizeBytes = 0;
			return S_FALSE;
		}

		return MultiByte_to_UTF16_choice(util::choice<0>{}, svValue, pOutputBuffer, nOutputBufferLengthBytes, oStringConversionOptions, pStringConversionResults);
	}
	inline HRESULT UTF16_to_MultiByte(
		std::wstring_view svValue,
		char* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		if (svValue.empty())
		{
			VLR_IF_NOT_NULL_DEREF(pStringConversionResults).m_nOuputSizeBytes = 0;
			return S_FALSE;
		}

		return UTF16_to_MultiByte_choice(util::choice<0>{}, svValue, pOutputBuffer, nOutputBufferLengthBytes, oStringConversionOptions, pStringConversionResults);
	}

	inline HRESULT MultiByte_to_UTF16(
		vlr::zstring_view svValue,
		wchar_t* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		auto oStringConversionOptions_Updated = StringConversionOptions{ oStringConversionOptions }.withNullTerminatedString(true);

		return MultiByte_to_UTF16(
			static_cast<std::string_view>(svValue),
			pOutputBuffer,
			nOutputBufferLengthBytes,
			oStringConversionOptions_Updated,
			pStringConversionResults);
	}
	inline HRESULT UTF16_to_MultiByte(
		vlr::wzstring_view svValue,
		char* pOutputBuffer,
		size_t nOutputBufferLengthBytes,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		auto oStringConversionOptions_Updated = StringConversionOptions{ oStringConversionOptions }.withNullTerminatedString(true);

		return UTF16_to_MultiByte(
			static_cast<std::wstring_view>(svValue),
			pOutputBuffer,
			nOutputBufferLengthBytes,
			oStringConversionOptions_Updated,
			pStringConversionResults);
	}

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

	inline HRESULT MultiByte_to_UTF16(
		const std::string& strValue,
		std::wstring& strOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		return MultiByte_to_UTF16(
			std::string_view{ strValue.c_str(), strValue.length() },
			strOutput,
			oStringConversionOptions,
			pStringConversionResults);
	}
	inline HRESULT UTF16_to_MultiByte(
		const std::wstring& strValue,
		std::string& strOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		return UTF16_to_MultiByte(
			std::wstring_view{ strValue.c_str(), strValue.length() },
			strOutput,
			oStringConversionOptions,
			pStringConversionResults);
	}

protected:
	// Note: The fallback here calls the internal conversion methods

	inline auto Inline_MultiByte_to_UTF16_StdString_choice(
		vlr::util::choice<10>&&,
		std::string_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr)
	{
		std::wstring strOutput;
		MultiByte_to_UTF16(svValue, strOutput, oStringConversionOptions, pStringConversionResults);
		return strOutput;
	}
	inline auto Inline_UTF16_to_MultiByte_StdString_choice(
		vlr::util::choice<10>&&,
		std::wstring_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr)
	{
		std::string strOutput;
		UTF16_to_MultiByte(svValue, strOutput, oStringConversionOptions, pStringConversionResults);
		return strOutput;
	}

	// choice<0> is the externally defined inline fallback. If this is defined, call by preference.

#if defined(VLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS)
	inline auto Inline_MultiByte_to_UTF16_StdString_choice(
		vlr::util::choice<0>&&,
		std::string_view svValue,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults) -> std::wstring
	{
		SResult sr;

		std::wstring swValue;
		sr = StringConversion::CExternalImpl::GetSharedInstance().Call_MultiByte_to_UTF16_StdString(svValue, swValue, oStringConversionOptions);
		if (sr != SResult::Success)
		{
			// Call the next potential conversion explicitly
			return Inline_MultiByte_to_UTF16_StdString_choice(
				vlr::util::choice<1>{},
				svValue,
				oStringConversionOptions,
				pStringConversionResults);
		}

		return swValue;
	}
	inline auto Inline_UTF16_to_MultiByte_StdString_choice(
		vlr::util::choice<0>&&,
		std::wstring_view svValue,
		const StringConversionOptions& oStringConversionOptions,
		StringConversionResults* pStringConversionResults) -> std::string
	{
		SResult sr;

		std::string saValue;
		sr = StringConversion::CExternalImpl::GetSharedInstance().Call_UTF16_to_MultiByte_StdString(svValue, saValue, oStringConversionOptions);
		if (sr != SResult::Success)
		{
			// Call the next potential conversion explicitly
			return Inline_UTF16_to_MultiByte_StdString_choice(
				vlr::util::choice<1>{},
				svValue,
				oStringConversionOptions,
				pStringConversionResults);
		}

		return saValue;
	}
#endif // defined(VLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS)

public:
	inline auto Inline_MultiByte_to_UTF16_StdString(
		std::string_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr ) noexcept
	{
		try
		{
			return Inline_MultiByte_to_UTF16_StdString_choice(util::choice<0>{}, svValue, oStringConversionOptions, pStringConversionResults);
		}
		catch (...)
		{
			return pStringConversionResults ? pStringConversionResults->m_swResultOnException : std::wstring{};
		}
	}
	inline auto Inline_UTF16_to_MultiByte_StdString(
		std::wstring_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr ) noexcept
	{
		try
		{
			return Inline_UTF16_to_MultiByte_StdString_choice(util::choice<0>{}, svValue, oStringConversionOptions, pStringConversionResults);
		}
		catch (...)
		{
			return pStringConversionResults ? pStringConversionResults->m_saResultOnException : std::string{};
		}
	}

	inline auto Inline_MultiByte_to_UTF16_StdString(
		const std::string& strValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr ) noexcept
	{
		try
		{
			return Inline_MultiByte_to_UTF16_StdString(static_cast<std::string_view>(strValue), oStringConversionOptions, pStringConversionResults);
		}
		catch (...)
		{
			return pStringConversionResults ? pStringConversionResults->m_swResultOnException : std::wstring{};
		}
	}
	inline auto Inline_UTF16_to_MultiByte_StdString(
		const std::wstring& strValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr ) noexcept
	{
		try
		{
			return Inline_UTF16_to_MultiByte_StdString(static_cast<std::wstring_view>(strValue), oStringConversionOptions, pStringConversionResults);
		}
		catch (...)
		{
			return pStringConversionResults ? pStringConversionResults->m_saResultOnException : std::string{};
		}
	}

#if VLR_CONFIG_INCLUDE_ATL_CString
	inline HRESULT MultiByte_to_UTF16(
		std::string_view svValue,
		CStringW& sOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr)
	{
		return win32::MultiByte_to_UTF16(svValue, sOutput, oStringConversionOptions, pStringConversionResults);
	}
	inline HRESULT UTF16_to_MultiByte(
		std::wstring_view svValue,
		CStringA& sOutput,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr)
	{
		return win32::UTF16_to_MultiByte(svValue, sOutput, oStringConversionOptions, pStringConversionResults);
	}

	inline auto Inline_MultiByte_to_UTF16_CString(
		std::string_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		CStringW sOutput;
		MultiByte_to_UTF16(svValue, sOutput, oStringConversionOptions, pStringConversionResults);
		return sOutput;
	}
	inline auto Inline_UTF16_to_MultiByte_CString(
		std::wstring_view svValue,
		const StringConversionOptions& oStringConversionOptions = {},
		StringConversionResults* pStringConversionResults = nullptr )
	{
		CStringA sOutput;
		UTF16_to_MultiByte(svValue, sOutput, oStringConversionOptions, pStringConversionResults);
		return sOutput;
	}
#endif // VLR_CONFIG_INCLUDE_ATL_CString

};

} // namespace util

} // namespace vlr
