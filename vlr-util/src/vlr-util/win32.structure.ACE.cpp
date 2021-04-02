#include "pch.h"
#include "win32.structure.ACE.h"

#include <sddl.h>

#include "UtilMacros.Assertions.h"
#include "AutoFreeResource.h"
#include "formatpf.h"
#include "win32.security.SIDs.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( structure )

vlr::tstring CAccessControlEntryBase::GetUniqueDescriptor() const
{
	auto sDescriptor = vlr::formatpf( _T( "%04X|%08X|%s" ),
		AceType(),
		GetAsApplicable_AccessMask().value_or( 0 ),
		GetAsApplicable_StringSid().value_or( _T( "" ) ) );
	return sDescriptor;
}

vlr::tstring CAccessControlEntryBase::GetDisplayString_Description() const
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
	//auto pSid = SidPtr();

	osStringSid = m_oSidInfo.GetStringSid();

	return S_OK;
}

HRESULT CAccessControlEntry_AccessAllowed::PopulateSidNameLookupResult( vlr::win32::security::SIDs::SPCSidNameLookupResult& spSidNameLookupResult_Result ) const
{
	return vlr::win32::security::SIDs::DoLookupAccountSid( nullptr, m_oSidInfo, spSidNameLookupResult_Result );
}

vlr::tstring CAccessControlEntry_AccessAllowed::GetStringSid()
{
	static const auto _tFailureValue = vlr::tstring{};

	if (m_oSidInfo.m_osStringSid.has_value())
	{
		return m_oSidInfo.m_osStringSid.value();
	}

	PopulateStringSid( m_oSidInfo.m_osStringSid );

	return m_oSidInfo.m_osStringSid.value_or( vlr::tstring{} );
}

vlr::tstring CAccessControlEntry_AccessAllowed::GetStringSid() const
{
	static const auto _tFailureValue = vlr::tstring{};

	if (m_oSidInfo.m_osStringSid.has_value())
	{
		return m_oSidInfo.m_osStringSid.value();
	}

	std::optional<vlr::tstring> osStringSid;
	PopulateStringSid( osStringSid );

	return osStringSid.value_or( vlr::tstring{} );
}

vlr::tstring CAccessControlEntry_AccessAllowed::GetDisplayString_Description() const
{
	auto sDisplayString = vlr::formatpf( _T( "ACCESS_ALLOWED_ACE; ACCESS_MASK %08X; Sid: %s" ),
		AccessMask(),
		m_oSidInfo.GetDisplay_Default() );
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
