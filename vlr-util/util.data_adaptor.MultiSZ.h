#pragma once

#include <list>
#include <string>
#include <vector>

#include "config.h"

#include "cpp_namespace.h"
#include "c_runtime.StringFunctions.h"
#include "zstring_view.h"
#include "UtilMacros.Assertions.h"

namespace vlr {

namespace util {

namespace data_adaptor {

template< typename TChar = TCHAR >
struct HelperFor_MultiSZ
{
	using std_string = std::basic_string<TChar>;

	[[deprecated("Use ToStructuredData() with span<> instead")]]
	HRESULT ToStructuredData(
		const TChar* pMultiSZ,
		std::vector<std_string>& oValueList);
	[[deprecated("Use ToStructuredData() with span<> instead")]]
	inline auto ToStructuredData(
		const TChar* pMultiSZ)
	{
		std::vector<std_string> oValueList;
#pragma warning(suppress: 4996) // 'function': was declared deprecated
		ToStructuredData(pMultiSZ, oValueList);
		return oValueList;
	}
	[[deprecated("Use ToStructuredData() with span<> instead")]]
	HRESULT ToStructuredData(
		const TChar* pMultiSZ,
		std::vector<vlr::basic_zstring_view<TChar>>& oValueCollection);

	HRESULT ToStructuredData(
		cpp::span<const TChar> spanMultiSZ,
		std::vector<vlr::basic_zstring_view<TChar>>& vecValueCollection);
	inline auto ToStructuredData(
		cpp::span<const TChar> spanMultiSZ)
	{
		std::vector<vlr::basic_zstring_view<TChar>> vecValueCollection;
		ToStructuredData(spanMultiSZ, vecValueCollection);
		return vecValueCollection;
	}
	HRESULT ToStructuredData(
		cpp::span<const TChar> spanMultiSZ,
		std::vector<std::basic_string<TChar>>& vecValueCollection_Result);

	HRESULT ToMultiSz(
		const std::vector<std_string>& oValueList,
		std::vector<BYTE>& arrData);
};

template< typename TChar >
inline auto MultiSZToStructuredData(const TChar* pMultiSZ)
{
#pragma warning(suppress: 4996) // 'function': was declared deprecated
	return HelperFor_MultiSZ<TChar>{}.ToStructuredData(pMultiSZ);
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	const TChar* pMultiSZ,
	std::vector<std_string>& oValueList)
{
	const auto* pBufferReadPtr = pMultiSZ;
	while (pBufferReadPtr[0] != TChar{ 0 })
	{
		auto nCurrentStringLen = c_runtime::StringFunctions::GetFunction_strlen<TChar>()(pBufferReadPtr);
		auto sCurrentString = std_string{ pBufferReadPtr, nCurrentStringLen };
		oValueList.push_back(sCurrentString);
		pBufferReadPtr += nCurrentStringLen + 1;
	}

	return S_OK;
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	const TChar* pMultiSZ,
	std::vector<vlr::basic_zstring_view<TChar>>& oValueCollection)
{
	const auto* pBufferReadPtr = pMultiSZ;
	while (pBufferReadPtr[0] != TChar{ 0 })
	{
		auto nCurrentStringLen = c_runtime::StringFunctions::GetFunction_strlen<TChar>()(pBufferReadPtr);
		auto svzCurrentString = vlr::basic_zstring_view<TChar>{ pBufferReadPtr, nCurrentStringLen, vlr::basic_zstring_view<TChar>::StringIsNullTerminated() };
		oValueCollection.push_back(svzCurrentString);
		pBufferReadPtr += nCurrentStringLen + 1;
	}

	return S_OK;
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	cpp::span<const TChar> spanMultiSZ,
	std::vector<vlr::basic_zstring_view<TChar>>& vecValueCollection)
{
	const auto* pszCurrentStringStart = spanMultiSZ.data();
	size_t nCurrentReadIndex = 0;
	size_t nSpanSize = spanMultiSZ.size();

	// A valid MultiSZ must have at least 2 characters for the double null terminator
	if (nSpanSize < 2)
	{
		return E_FAIL;
	}

	do
	{
		if (nCurrentReadIndex >= nSpanSize)
		{
			// Exhausted span without finding double null terminator
			// Note: Empty MultiSZ is invalid, because it must be double null terminated, so we should have 
			// found a null terminator before exhausting the span.
			return E_FAIL;
		}

		size_t nCurrentStringLen = 0;
		while (pszCurrentStringStart[nCurrentReadIndex + nCurrentStringLen] != TChar{ 0 })
		{
			nCurrentStringLen++;
			if (nCurrentReadIndex + nCurrentStringLen >= nSpanSize)
			{
				// Exhausted span without finding null terminator
				return E_FAIL;
			}
		}
		if (nCurrentStringLen == 0)
		{
			// Found double null terminator, so we are done
			return S_OK;
		}
		// At this point, we have found a null terminator for the current string, so we can add it to the collection.

		// Create and add the zstring_view
		auto svzCurrentString = vlr::basic_zstring_view<TChar>{ pszCurrentStringStart + nCurrentReadIndex, nCurrentStringLen, vlr::basic_zstring_view<TChar>::StringIsNullTerminated() };
		vecValueCollection.push_back(svzCurrentString);

		// Move to the next string (skip current string + null terminator)
		nCurrentReadIndex += nCurrentStringLen + 1;
	} while (true);

	// Should not reach here, but just in case, return unexpected error
	VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(E_UNEXPECTED);
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	cpp::span<const TChar> spanMultiSZ,
	std::vector<std::basic_string<TChar>>& vecValueCollection_Result)
{
	HRESULT hr;

	std::vector<vlr::basic_zstring_view<TChar>> vecValueCollection;
	hr = ToStructuredData(spanMultiSZ, vecValueCollection);
	VLR_ON_ERROR_RETURN_VALUE(hr);

	for (const auto& svzValue : vecValueCollection)
	{
		vecValueCollection_Result.push_back(svzValue.toStdString());
	}

	return S_OK;
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToMultiSz(
	const std::vector<std_string>& oValueList,
	std::vector<BYTE>& arrData)
{
	size_t nAggregateCharCount = 0;
	for (const auto& sValue : oValueList)
	{
		nAggregateCharCount += sValue.length() + 1;
	}
	// One additional for double NULL terminator
	nAggregateCharCount += 1;

	size_t nAggregateByteCount = nAggregateCharCount * sizeof(TChar);
	arrData.resize(nAggregateByteCount);

	BYTE* pDataWritePoint = arrData.data();
	size_t nCountWrittenBytes = 0;

	for (const auto& sValue : oValueList)
	{
		// Note: Writing string plus NULL terminator
		size_t nStringLenPlusTerminatorBytes = (sValue.length() + 1) * sizeof(TChar);

		memcpy_s(pDataWritePoint, arrData.size() - nCountWrittenBytes, sValue.c_str(), nStringLenPlusTerminatorBytes);
		nCountWrittenBytes += nStringLenPlusTerminatorBytes;
		pDataWritePoint += nStringLenPlusTerminatorBytes;
	}
	// Add extra NULL terminator
	//VLR_ASSERT_COMPARE_OR_RETURN_EUNEXPECTED(arrData.size() - nCountWrittenBytes, == , 1);
	*pDataWritePoint = 0;

	return S_OK;
}

} // namespace data_adaptor

} // namespace util

} // namespace vlr
