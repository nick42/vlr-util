#pragma once

#include <functional>
#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "ActionOnDestruction.h"

NAMESPACE_BEGIN( vlr )

template< typename TResource, typename TFreeResult >
class CAutoFreeResource
	: public CActionOnDestruction<TFreeResult>
{
	using base_class = CActionOnDestruction<TFreeResult>;

protected:
	TResource m_tResource = TResource{};
	std::function<TFreeResult( TResource& tResource )> m_fFreeResource;

public:
	CAutoFreeResource( const TResource& tResource, const std::function<TFreeResult( TResource& tResource )>& fFreeResource )
		: m_tResource{ tResource }
		, m_fFreeResource{ fFreeResource }
	{
		base_class::m_fAction = [this] { return m_fFreeResource( m_tResource ); };
	}
	// To ensure the resources are valid at time of call, this needs to be called before this subclass goes away...
	virtual ~CAutoFreeResource()
	{
		base_class::DoActionAndClear();
	}
};

template< typename TResource >
inline auto MakeAutoCleanup_viaLocalFree( const TResource& tResource )
{
	auto fAction = [=]
	{
		LocalFree( reinterpret_cast<HLOCAL>(tResource) );
	};
	return MakeActionOnDestruction( fAction );
}

inline auto MakeAutoCleanup_viaCloseHandle( HANDLE hHandle )
{
	auto fAction = [=]
	{
		::CloseHandle( hHandle );
	};
	return MakeActionOnDestruction( fAction );
}

NAMESPACE_END //( vlr )
