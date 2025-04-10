#include "pch.h"
#include "util.StringConversion.h"

#include "UtilMacros.Assertions.h"
#include "UtilMacros.General.h"

namespace vlr {

namespace util {

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

} // namespace util

} // namespace vlr
