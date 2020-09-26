#pragma once

#include <list>
#include <string>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "c_runtime.StringFunctions.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( data_adaptor )

template< typename TChar = TCHAR >
struct HelperFor_MultiSZ
{
	using std_string = std::basic_string<TChar>;

	HRESULT ToStructuredData(
		const TChar* pMultiSZ,
		std::list<std_string>& oValueList );
	decltype(auto) ToStructuredData(
		const TChar* pMultiSZ )
	{
		std::list<std_string> oValueList;
		ToStructuredData( pMultiSZ, oValueList );
		return oValueList;
	}

};

template< typename TChar >
inline decltype(auto) MultiSZToStructuredData( const TChar* pMultiSZ )
{
	return HelperFor_MultiSZ<TChar>{}.ToStructuredData( pMultiSZ );
}

template< typename TChar >
HRESULT HelperFor_MultiSZ<TChar>::ToStructuredData(
	const TChar* pMultiSZ,
	std::list<std_string>& oValueList )
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

NAMESPACE_END //( data_adaptor )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
