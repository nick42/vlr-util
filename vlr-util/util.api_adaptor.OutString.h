#pragma once

#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"

namespace vlr {

namespace util {

namespace api_adaptor {

namespace detail {

template< typename TString, typename TStringBufferPtr = LPTSTR >
struct StringBufferAccess
{
	static TStringBufferPtr GetBufferPtr( TString& tString );
};

template<>
LPTSTR StringBufferAccess<vlr::tstring, LPTSTR>::GetBufferPtr( vlr::tstring& tString )
{
	return tString.data();
}

template<>
LPSTR StringBufferAccess<CStringA, LPSTR>::GetBufferPtr( CStringA& tString )
{
	return tString.GetBuffer();
}

} // namespace detail

template< typename TString, typename TStringBufferPtr = LPTSTR, typename TStringSize = DWORD >
class COutString
{
protected:
	TString& m_tString;
	std::optional<TStringSize> m_onAllocatedLength;
	TStringSize m_nValueLength = 0;

public:
	TStringBufferPtr GetBufferPtr() const;
	TStringSize& GetSizeParam()
	{
		return m_nValueLength;
	}

protected:
	HRESULT AllocateBuffer( TStringSize nSize );
	HRESULT OnDestroy_CleanupBuffer();

public:
	COutString( TString& tString )
		: m_tString{ tString }
	{}
};

} // namespace api_adaptor

} // namespace util

} // namespace vlr
