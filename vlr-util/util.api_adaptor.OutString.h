#pragma once

#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( api_adaptor )

NAMESPACE_BEGIN( detail )

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

NAMESPACE_END //( detail )

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

NAMESPACE_END //( api_adaptor )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
