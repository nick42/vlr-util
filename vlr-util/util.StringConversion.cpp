#include "pch.h"
#include "util.StringConversion.h"

#include "UtilMacros.Assertions.h"
#include "UtilMacros.General.h"
#include "util.range_checked_cast.h"
#include "util.CStringBufferAccess.h"

namespace vlr {

namespace util {

#if defined(WIN32)

HRESULT CStringConversion::MultiByte_to_UTF16_Win32(
	std::string_view svValue,
	wchar_t* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions,
	StringConversionResults* pStringConversionResults)
{
	bool bInputBufferShouldIncludeNullTerminator = true
		&& oStringConversionOptions.m_bInputStringIsNullTerminated
		&& (!oStringConversionOptions.m_bGenerateResultNotNullTerminated)
		;
	auto nEffectiveInputBufferLengthChars = svValue.length() + (bInputBufferShouldIncludeNullTerminator ? 1 : 0);

	auto nOutputBufferLengthChars = (nOutputBufferLengthBytes / sizeof(wchar_t));

	bool bNeedAdditionalSpaceInOutputBufferToAddNullTerminator = true
		&& (!bInputBufferShouldIncludeNullTerminator)
		&& (!oStringConversionOptions.m_bGenerateResultNotNullTerminated)
		&& (!oStringConversionOptions.m_bInputStringIsNullTerminated)
		;
	size_t nOutputBufferLengthAdjustmentBytes = bNeedAdditionalSpaceInOutputBufferToAddNullTerminator ? sizeof(wchar_t) : 0;
	// Be sure we don't underflow, if we passed a zero for buffer length
	auto nUsableOutputBufferLengthChars = (nOutputBufferLengthChars == 0) ? 0 : nOutputBufferLengthChars - nOutputBufferLengthAdjustmentBytes;

	auto nResult = ::MultiByteToWideChar(
		oStringConversionOptions.GetCodePageIdentifier(),
		oStringConversionOptions.OnMultiByteToWideChar_GetFlags(),
		svValue.data(),
		range_checked_cast<int>(nEffectiveInputBufferLengthChars),
		pOutputBuffer,
		range_checked_cast<int>(nUsableOutputBufferLengthChars));
	if (nResult == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	VLR_IF_NOT_NULL(pStringConversionResults)->m_nOuputSizeBytes = (range_checked_cast<size_t>(nResult) * sizeof(wchar_t)) + nOutputBufferLengthAdjustmentBytes;
	if (range_checked_cast<size_t>(nResult) > nUsableOutputBufferLengthChars)
	{
		return S_FALSE;
	}

	if (bNeedAdditionalSpaceInOutputBufferToAddNullTerminator)
	{
		ASSERT(range_checked_cast<size_t>(nResult) < nOutputBufferLengthChars);
		pOutputBuffer[nResult] = L'\0';
	}

	return S_OK;
}

HRESULT CStringConversion::UTF16_to_MultiByte_Win32(
	std::wstring_view svValue,
	char* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions,
	StringConversionResults* pStringConversionResults)
{
	bool bInputBufferShouldIncludeNullTerminator = true
		&& oStringConversionOptions.m_bInputStringIsNullTerminated
		&& (!oStringConversionOptions.m_bGenerateResultNotNullTerminated)
		;
	auto nEffectiveInputBufferLengthChars = svValue.length() + (bInputBufferShouldIncludeNullTerminator ? 1 : 0);

	auto nOutputBufferLengthChars = (nOutputBufferLengthBytes / sizeof(char));

	bool bNeedAdditionalSpaceInOutputBufferToAddNullTerminator = true
		&& (!bInputBufferShouldIncludeNullTerminator)
		&& (!oStringConversionOptions.m_bGenerateResultNotNullTerminated)
		&& (!oStringConversionOptions.m_bInputStringIsNullTerminated)
		;
	size_t nOutputBufferLengthAdjustmentBytes = bNeedAdditionalSpaceInOutputBufferToAddNullTerminator ? sizeof(char) : 0;
	// Be sure we don't underflow, if we passed a zero for buffer length
	auto nUsableOutputBufferLengthChars = (nOutputBufferLengthChars == 0) ? 0 : nOutputBufferLengthChars - nOutputBufferLengthAdjustmentBytes;

	auto nResult = ::WideCharToMultiByte(
		oStringConversionOptions.GetCodePageIdentifier(),
		oStringConversionOptions.OnWideCharToMultiByte_GetFlags(),
		svValue.data(),
		range_checked_cast<int>(nEffectiveInputBufferLengthChars),
		pOutputBuffer,
		range_checked_cast<int>(nUsableOutputBufferLengthChars),
		oStringConversionOptions.OnWideCharToMultiByte_GetDefaultChar(),
		oStringConversionOptions.OnWideCharToMultiByte_GetUsedDefaultChar(pStringConversionResults));
	if (nResult == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	VLR_IF_NOT_NULL(pStringConversionResults)->m_nOuputSizeBytes = (range_checked_cast<size_t>(nResult) * sizeof(char)) + nOutputBufferLengthAdjustmentBytes;
	if (range_checked_cast<size_t>(nResult) > nUsableOutputBufferLengthChars)
	{
		return S_FALSE;
	}

	if (bNeedAdditionalSpaceInOutputBufferToAddNullTerminator)
	{
		ASSERT(range_checked_cast<size_t>(nResult) < nOutputBufferLengthChars);
		pOutputBuffer[nResult] = '\0';
	}

	return S_OK;
}

#endif // defined(WIN32)

#if defined(VLR_FALLBACK_Inline_MultiByte_to_UTF16_StdString)

HRESULT CStringConversion::MultiByte_to_UTF16_choice(
	vlr::util::choice<0>&&,
	std::string_view svValue,
	wchar_t* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions,
	StringConversionResults* /*pStringConversionResults*/)
{
	std::wstring swValue = VLR_FALLBACK_Inline_MultiByte_to_UTF16_StdString(svValue, oStringConversionOptions);
	if (nOutputBufferLengthBytes < swValue.length() + sizeof(wchar_t))
	{
		return E_FAIL;
	}

#if defined(WIN32)
	wcscpy_s(pOutputBuffer, nOutputBufferLengthBytes / sizeof(wchar_t), swValue.c_str());
#else
	wcscpy(pOutputBuffer, swValue.c_str());
#endif

	return S_OK;
}

#endif // defined(VLR_FALLBACK_Inline_MultiByte_to_UTF16_StdString)

#if defined(VLR_FALLBACK_Inline_UTF16_to_MultiByte_StdString)

HRESULT CStringConversion::UTF16_to_MultiByte_choice(
	vlr::util::choice<0>&&,
	std::wstring_view svValue,
	char* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions,
	StringConversionResults* /*pStringConversionResults*/)
{
	std::string saValue = VLR_FALLBACK_Inline_UTF16_to_MultiByte_StdString(svValue, oStringConversionOptions);
	if (nOutputBufferLengthBytes < saValue.length() + sizeof(char))
	{
		return E_FAIL;
	}

#if defined(WIN32)
	strcpy_s(pOutputBuffer, nOutputBufferLengthBytes / sizeof(char), saValue.c_str());
#else
	strcpy(pOutputBuffer, swValue.c_str());
#endif

	return S_OK;
}

#endif // defined(VLR_FALLBACK_Inline_MultiByte_to_UTF16_StdString)

HRESULT CStringConversion::MultiByte_to_UTF16(
	std::string_view svValue,
	wchar_t* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	if (svValue.empty())
	{
		return S_FALSE;
	}

	return MultiByte_to_UTF16_choice(util::choice<0>{}, svValue, pOutputBuffer, nOutputBufferLengthBytes, oStringConversionOptions, pStringConversionResults);
}

HRESULT CStringConversion::UTF16_to_MultiByte(
	std::wstring_view svValue,
	char* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	if (svValue.empty())
	{
		return S_FALSE;
	}

	return UTF16_to_MultiByte_choice(util::choice<0>{}, svValue, pOutputBuffer, nOutputBufferLengthBytes, oStringConversionOptions, pStringConversionResults);
}

HRESULT CStringConversion::MultiByte_to_UTF16(
	vlr::zstring_view svValue,
	wchar_t* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	auto oStringConversionOptions_Updated = StringConversionOptions{ oStringConversionOptions }.withNullTerminatedString(true);

	return MultiByte_to_UTF16(
		static_cast<std::string_view>(svValue),
		pOutputBuffer,
		nOutputBufferLengthBytes,
		oStringConversionOptions_Updated,
		pStringConversionResults );
}

HRESULT CStringConversion::UTF16_to_MultiByte(
	vlr::wzstring_view svValue,
	char* pOutputBuffer,
	size_t nOutputBufferLengthBytes,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	auto oStringConversionOptions_Updated = StringConversionOptions{ oStringConversionOptions }.withNullTerminatedString(true);

	return UTF16_to_MultiByte(
		static_cast<std::wstring_view>(svValue),
		pOutputBuffer,
		nOutputBufferLengthBytes,
		oStringConversionOptions_Updated,
		pStringConversionResults );
}

HRESULT CStringConversion::MultiByte_to_UTF16(
	std::string_view svValue,
	std::wstring& strOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	HRESULT hr;

	auto nOutputBufferSizeChars = svValue.length();

	while (true)
	{
		strOutput.resize(nOutputBufferSizeChars);
		auto nOutputBufferSizeBytes = nOutputBufferSizeChars * sizeof(wchar_t);

		auto oStringConversionOptions_Local = StringConversionOptions{ oStringConversionOptions }.withGenerateResultNotNullTerminated(true);
		auto oStringConversionResults = StringConversionResults{};

		hr = MultiByte_to_UTF16(
			svValue,
			strOutput.data(),
			nOutputBufferSizeBytes,
			oStringConversionOptions_Local,
			&oStringConversionResults);
		VLR_ON_HR_NON_S_OK__RETURN_HRESULT(hr);

		auto nOutputSizeChars = oStringConversionResults.m_nOuputSizeBytes / sizeof(wchar_t);
		if (oStringConversionResults.m_nOuputSizeBytes > nOutputBufferSizeBytes)
		{
			nOutputBufferSizeChars = nOutputSizeChars;
			continue;
		}

		// resize() again to truncate as necessary
		strOutput.resize(nOutputSizeChars);

		VLR_IF_NOT_NULL_DEREF(pStringConversionResults) = oStringConversionResults;

		break;
	}

	return S_OK;
}

HRESULT CStringConversion::UTF16_to_MultiByte(
	std::wstring_view svValue,
	std::string& strOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/)
{
	HRESULT hr;

	// Note: The maximum length of a converted string, I believe, is 4x the (chars) length of the source string.
	// ie: If every source char is converted into an extended MBCS 4byte character.
	// If longer is possible, than this can fail.
	auto nOutputBufferSizeChars = svValue.length() * 4;

	while (true)
	{
		strOutput.resize(nOutputBufferSizeChars);
		auto nOutputBufferSizeBytes = nOutputBufferSizeChars * sizeof(char);

		auto oStringConversionOptions_Local = StringConversionOptions{ oStringConversionOptions }.withGenerateResultNotNullTerminated(true);
		auto oStringConversionResults = StringConversionResults{};

		hr = UTF16_to_MultiByte(
			svValue,
			strOutput.data(),
			nOutputBufferSizeBytes,
			oStringConversionOptions_Local,
			&oStringConversionResults);
		VLR_ON_HR_NON_S_OK__RETURN_HRESULT(hr);

		auto nOutputSizeChars = oStringConversionResults.m_nOuputSizeBytes / sizeof(char);
		if (oStringConversionResults.m_nOuputSizeBytes > nOutputBufferSizeBytes)
		{
			nOutputBufferSizeChars = nOutputSizeChars;
			continue;
		}

		// resize() again to truncate as necessary
		strOutput.resize(nOutputSizeChars);

		VLR_IF_NOT_NULL_DEREF(pStringConversionResults) = oStringConversionResults;

		break;
	}

	return S_OK;
}

HRESULT CStringConversion::MultiByte_to_UTF16(
	const std::string& strValue,
	std::wstring& strOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	return MultiByte_to_UTF16(
		std::string_view{ strValue.c_str(), strValue.length() },
		strOutput,
		oStringConversionOptions,
		pStringConversionResults );
}

HRESULT CStringConversion::UTF16_to_MultiByte(
	const std::wstring& strValue,
	std::string& strOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	return UTF16_to_MultiByte(
		std::wstring_view{ strValue.c_str(), strValue.length() },
		strOutput,
		oStringConversionOptions,
		pStringConversionResults );
}

#if VLR_CONFIG_INCLUDE_ATL_CString

HRESULT CStringConversion::MultiByte_to_UTF16(
	std::string_view svValue,
	CStringW& sOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	HRESULT hr;

	auto nOutputBufferSizeChars = svValue.length();

	while (true)
	{
		auto oOutputBufferAccess = GetCStringBufferAccess( sOutput, range_checked_cast<int>(nOutputBufferSizeChars) );
		auto nOutputBufferSizeBytes = nOutputBufferSizeChars * sizeof( wchar_t );

		auto oStringConversionOptions_Local = StringConversionOptions{ oStringConversionOptions }.withGenerateResultNotNullTerminated(true);
		auto oStringConversionResults = StringConversionResults{};

		hr = MultiByte_to_UTF16(
			svValue,
			oOutputBufferAccess,
			nOutputBufferSizeBytes,
			oStringConversionOptions_Local,
			&oStringConversionResults );
		VLR_ON_HR_NON_S_OK__RETURN_HRESULT( hr );

		auto nOutputSizeChars = oStringConversionResults.m_nOuputSizeBytes / sizeof( wchar_t );
		if (oStringConversionResults.m_nOuputSizeBytes > nOutputBufferSizeBytes)
		{
			nOutputBufferSizeChars = nOutputSizeChars;
			continue;
		}

		oOutputBufferAccess.ReleaseBufferPtr( range_checked_cast<int>(nOutputSizeChars) );

		VLR_IF_NOT_NULL_DEREF( pStringConversionResults ) = oStringConversionResults;

		break;
	}

	return S_OK;
}

HRESULT CStringConversion::UTF16_to_MultiByte(
	std::wstring_view svValue,
	CStringA& sOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/ )
{
	HRESULT hr;

	auto nOutputBufferSizeChars = svValue.length();

	while (true)
	{
		auto oOutputBufferAccess = GetCStringBufferAccess( sOutput, range_checked_cast<int>(nOutputBufferSizeChars) );
		auto nOutputBufferSizeBytes = nOutputBufferSizeChars * sizeof( char );

		auto oStringConversionOptions_Local = StringConversionOptions{ oStringConversionOptions }.withGenerateResultNotNullTerminated(true);
		auto oStringConversionResults = StringConversionResults{};

		hr = UTF16_to_MultiByte(
			svValue,
			oOutputBufferAccess,
			nOutputBufferSizeBytes,
			oStringConversionOptions_Local,
			&oStringConversionResults );
		VLR_ON_HR_NON_S_OK__RETURN_HRESULT( hr );

		auto nOutputSizeChars = oStringConversionResults.m_nOuputSizeBytes / sizeof( char );
		if (oStringConversionResults.m_nOuputSizeBytes > nOutputBufferSizeBytes)
		{
			nOutputBufferSizeChars = nOutputSizeChars;
			continue;
		}

		oOutputBufferAccess.ReleaseBufferPtr( range_checked_cast<int>(nOutputSizeChars) );

		VLR_IF_NOT_NULL_DEREF( pStringConversionResults ) = oStringConversionResults;

		break;
	}

	return S_OK;
}

#endif // VLR_CONFIG_INCLUDE_ATL_CString

} // namespace util

} // namespace vlr
