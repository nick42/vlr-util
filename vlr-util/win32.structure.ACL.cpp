#include "pch.h"
#include "vlr/win32.structure.ACL.h"

#include "vlr/UtilMacros.Assertions.h"
#include "vlr/win32.structure.ACE.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( structure )

bool CAccessorFor_ACL::HasMetaValue_EntirelyInherited() const
{
	static constexpr auto _tFailureValue = false;

	for (decltype(AceCount) nAceIndex = 0; nAceIndex < AceCount; ++nAceIndex)
	{
		LPVOID pvAce = nullptr;
		auto bResult = ::GetAce( const_cast<ACL*>(static_cast<const ACL*>(this)), nAceIndex, &pvAce );
		ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( bResult );

		auto&& oACE_HEADER = vlr::win32::structure::MakeStructureAccessor( reinterpret_cast<const ACE_HEADER*>(pvAce) );

		if (!oACE_HEADER.HasFlag_Inherited())
		{
			return false;
		}
	}

	return true;
}

bool CAccessControlList::IsEffectivelyIdenticalTo( const CAccessControlList& oOther )
{
	auto&& lhsIter = m_oAccessControlEntryList.begin();
	auto&& lhsEnd = m_oAccessControlEntryList.end();
	auto&& rhsIter = oOther.m_oAccessControlEntryList.begin();
	auto&& rhsEnd = oOther.m_oAccessControlEntryList.end();

	do
	{
		if (lhsIter == lhsEnd)
		{
			break;
		}
		if (rhsIter == rhsEnd)
		{
			break;
		}
		const auto& lhsACE = *(*lhsIter++);
		const auto& rhsACE = *(*rhsIter++);
		if (!lhsACE.IsIdentical( rhsACE ))
		{
			return false;
		}
	} while (true);

	if (lhsIter != lhsEnd)
	{
		return false;
	}
	if (rhsIter != rhsEnd)
	{
		return false;
	}

	return true;
}

HRESULT CAccessControlList::Initialize( const ACL* pACL )
{
	static constexpr auto _tFailureValue = E_FAIL;

	auto&& oACL = MakeStructureAccessor( pACL );

	m_oAccessControlEntryList.reserve( oACL.AceCount );

	for (decltype(oACL.AceCount) nAceIndex = 0; nAceIndex < oACL.AceCount; ++nAceIndex)
	{
		LPVOID pvAce = nullptr;
		auto bResult = ::GetAce( const_cast<ACL*>(pACL), nAceIndex, &pvAce );
		ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( bResult );

		auto spStructure = MakeStructureSP_AccessControlEntry( pvAce );
		ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( spStructure );

		m_oAccessControlEntryList.push_back( spStructure );
	}

	return S_OK;
}

NAMESPACE_END //( structure )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
