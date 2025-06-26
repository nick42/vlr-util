#include "pch.h"
#include "util.win32.StringConversion.h"

#include <vlr-util/UtilMacros.General.h>
#include "util.CStringBufferAccess.h"

namespace vlr {

namespace util {

namespace win32 {

HRESULT MultiByte_to_UTF16(
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

HRESULT UTF16_to_MultiByte(
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

#if VLR_CONFIG_INCLUDE_ATL_CString

HRESULT MultiByte_to_UTF16(
	std::string_view svValue,
	CStringW& sOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/)
{
	HRESULT hr;

	auto nOutputBufferSizeChars = svValue.length();

	while (true)
	{
		auto oOutputBufferAccess = GetCStringBufferAccess(sOutput, range_checked_cast<int>(nOutputBufferSizeChars));
		auto nOutputBufferSizeBytes = nOutputBufferSizeChars * sizeof(wchar_t);

		auto oStringConversionOptions_Local = StringConversionOptions{ oStringConversionOptions }.withGenerateResultNotNullTerminated(true);
		auto oStringConversionResults = StringConversionResults{};

		hr = MultiByte_to_UTF16(
			svValue,
			oOutputBufferAccess,
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

		oOutputBufferAccess.ReleaseBufferPtr(range_checked_cast<int>(nOutputSizeChars));

		VLR_IF_NOT_NULL_DEREF(pStringConversionResults) = oStringConversionResults;

		break;
	}

	return S_OK;
}

HRESULT UTF16_to_MultiByte(
	std::wstring_view svValue,
	CStringA& sOutput,
	const StringConversionOptions& oStringConversionOptions /*= {}*/,
	StringConversionResults* pStringConversionResults /*= nullptr*/)
{
	HRESULT hr;

	auto nOutputBufferSizeChars = svValue.length();

	while (true)
	{
		auto oOutputBufferAccess = GetCStringBufferAccess(sOutput, range_checked_cast<int>(nOutputBufferSizeChars));
		auto nOutputBufferSizeBytes = nOutputBufferSizeChars * sizeof(char);

		auto oStringConversionOptions_Local = StringConversionOptions{ oStringConversionOptions }.withGenerateResultNotNullTerminated(true);
		auto oStringConversionResults = StringConversionResults{};

		hr = UTF16_to_MultiByte(
			svValue,
			oOutputBufferAccess,
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

		oOutputBufferAccess.ReleaseBufferPtr(range_checked_cast<int>(nOutputSizeChars));

		VLR_IF_NOT_NULL_DEREF(pStringConversionResults) = oStringConversionResults;

		break;
	}

	return S_OK;
}

#endif // VLR_CONFIG_INCLUDE_ATL_CString

} // namespace win32

} // namespace util

} // namespace vlr
