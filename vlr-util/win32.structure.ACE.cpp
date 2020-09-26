#include "pch.h"
#include "vlr/win32.structure.ACE.h"

#include <sddl.h>

#include "vlr/UtilMacros.Assertions.h"
#include "vlr/AutoFreeResource.h"
#include "vlr/formatpf.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( structure )

vlr::tstring CAccessControlEntry_Unknown::GetDisplayString_Description() const
{
	auto sDisplayString = vlr::formatpf( _T( "ACE type %d; size %d" ),
		AceType(),
		AceSize() );
	return sDisplayString;
}

HRESULT CAccessControlEntry_AccessAllowed::PopulateStringSid( std::optional<vlr::tstring>& osStringSid ) const
{
	static const auto _tFailureValue = E_FAIL;

	// Convert binary SID to string SID

	//auto pAce = reinterpret_cast<ACCESS_ALLOWED_ACE*>(m_oEntryData.data());
	//auto pSidStart = &pAce->SidStart;
	//auto pSid = reinterpret_cast<PSID>(pSidStart);
	auto pSid = SidPtr();

	LPTSTR pszStringSid = nullptr;
	auto bSuccess = ::ConvertSidToStringSid(
		const_cast<PSID>(pSid),
		&pszStringSid );
	ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( bSuccess );
	ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( pszStringSid );
	auto oOnDestroy_FreeStringSid = MakeAutoCleanup_viaLocalFree( pszStringSid );

	osStringSid = vlr::tstring{ pszStringSid };

	return S_OK;
}

vlr::tstring CAccessControlEntry_AccessAllowed::GetStringSid()
{
	static const auto _tFailureValue = vlr::tstring{};

	if (m_osStringSid.has_value())
	{
		return m_osStringSid.value();
	}

	PopulateStringSid( m_osStringSid );

	return m_osStringSid.value_or( vlr::tstring{} );
}

vlr::tstring CAccessControlEntry_AccessAllowed::GetStringSid() const
{
	static const auto _tFailureValue = vlr::tstring{};

	if (m_osStringSid.has_value())
	{
		return m_osStringSid.value();
	}

	std::optional<vlr::tstring> osStringSid;
	PopulateStringSid( osStringSid );

	return m_osStringSid.value_or( vlr::tstring{} );
}

vlr::tstring CAccessControlEntry_AccessAllowed::GetDisplayString_Description() const
{
	auto sDisplayString = vlr::formatpf( _T( "ACCESS_ALLOWED_ACE; ACCESS_MASK %08X; Sid: %s" ),
		AccessMask(),
		GetStringSid() );
	return sDisplayString;
}

SPCAccessControlEntryBase MakeStructureSP_AccessControlEntry( LPVOID pvACE )
{
	auto&& oAceHeader = MakeStructureAccessor( static_cast<const ACE_HEADER*>(pvACE) );

	switch (oAceHeader.AceType)
	{
	case ACCESS_ALLOWED_ACE_TYPE:
		return std::make_shared<CAccessControlEntry_AccessAllowed>( pvACE );

	default:
		return std::make_shared<CAccessControlEntry_Unknown>( pvACE );
	}
}

NAMESPACE_END //( structure )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
