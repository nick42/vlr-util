#pragma once

#include <list>
#include <string>
#include <vector>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "c_runtime.StringFunctions.h"

#include "zstring_view.h"

namespace vlr {

namespace util {

namespace data_adaptor {

template< typename TChar = TCHAR >
struct HelperFor_MultiSZ
{
	using std_string = std::basic_string<TChar>;

	HRESULT ToStructuredData(
		const TChar* pMultiSZ,
		std::vector<std_string>& oValueList );
	inline auto ToStructuredData(
		const TChar* pMultiSZ )
	{
		std::vector<std_string> oValueList;
		ToStructuredData( pMultiSZ, oValueList );
		return oValueList;
	}
	HRESULT ToStructuredData(
		const TChar* pMultiSZ,
		std::vector<vlr::basic_zstring_view<TChar>>& oValueCollection );

	HRESULT ToMultiSz(
		const std::vector<std_string>& oValueList,
		std::vector<BYTE>& arrData);
};

template< typename TChar >
inline auto MultiSZToStructuredData( const TChar* pMultiSZ )
{
	return HelperFor_MultiSZ<TChar>{}.ToStructuredData( pMultiSZ );
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	const TChar* pMultiSZ,
	std::vector<std_string>& oValueList )
{
	const auto* pBufferReadPtr = pMultiSZ;
	while (pBufferReadPtr[0] != TChar{ 0 })
	{
		auto nCurrentStringLen = c_runtime::StringFunctions::GetFunction_strlen<TChar>()( pBufferReadPtr );
		auto sCurrentString = std_string{ pBufferReadPtr, nCurrentStringLen };
		oValueList.push_back( sCurrentString );
		pBufferReadPtr += nCurrentStringLen + 1;
	}

	return S_OK;
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	const TChar* pMultiSZ,
	std::vector<vlr::basic_zstring_view<TChar>>& oValueCollection )
{
	const auto* pBufferReadPtr = pMultiSZ;
	while (pBufferReadPtr[0] != TChar{ 0 })
	{
		auto nCurrentStringLen = c_runtime::StringFunctions::GetFunction_strlen<TChar>()(pBufferReadPtr);
		auto svzCurrentString = vlr::basic_zstring_view<TChar>{ pBufferReadPtr, nCurrentStringLen, vlr::basic_zstring_view<TChar>::StringIsNullTerminated() };
		oValueCollection.push_back( svzCurrentString );
		pBufferReadPtr += nCurrentStringLen + 1;
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
