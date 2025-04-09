#pragma once

// Note: Exclude this header entirely if not including CString references
#if VLR_CONFIG_INCLUDE_ATL_CString

#include <atlstr.h>

#include "config.h"

namespace vlr {

namespace util {

template< typename BaseType, class StringTraits >
class CStringBufferAccess
{
public:
	using TStringAccessing = CStringT<BaseType, StringTraits>;

protected:
	TStringAccessing& m_sValue;
	BaseType* m_pBuffer = nullptr;

public:
	HRESULT PopulateBufferPtr( int nMinBufferLength = 0 )
	{
		m_pBuffer = m_sValue.GetBuffer( nMinBufferLength );

		return S_OK;
	}
	HRESULT ReleaseBufferPtr( int nStringLength = -1 )
	{
		if (m_pBuffer == nullptr)
		{
			return S_FALSE;
		}
		m_sValue.ReleaseBuffer( nStringLength );
		m_pBuffer = nullptr;

		return S_OK;
	}
	inline operator BaseType* () const
	{
		return m_pBuffer;
	}

public:
	CStringBufferAccess( TStringAccessing& sValue, int nMinBufferLength = 0 )
		: m_sValue{ sValue }
	{
		PopulateBufferPtr( nMinBufferLength );
	}
	~CStringBufferAccess()
	{
		ReleaseBufferPtr();
	}
};

template< typename BaseType, class StringTraits >
auto GetCStringBufferAccess( CStringT<BaseType, StringTraits>& sValue, int nMinBufferLength = 0 )
{
	return CStringBufferAccess{ sValue, nMinBufferLength };
}

} // namespace util

} // namespace vlr

#endif // VLR_CONFIG_INCLUDE_ATL_CString
