#pragma once

#include <memory>
#include <list>
#include <vector>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/win32.structure.ACE.h"
#include "vlr/logging.MessageContext.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( structure )

struct CAccessorFor_ACL
	: public ACL
{
	bool HasMetaValue_EntirelyInherited() const;

	CAccessorFor_ACL( const CAccessorFor_ACL& ) = delete;
};

inline decltype(auto) MakeStructureAccessor( ACL* pFindData )
{
	static_assert(sizeof( CAccessorFor_ACL ) == sizeof( ACL ));
	return *reinterpret_cast<CAccessorFor_ACL*>(pFindData);
}

inline decltype(auto) MakeStructureAccessor( const ACL* pFindData )
{
	static_assert(sizeof( CAccessorFor_ACL ) == sizeof( ACL ));
	return *reinterpret_cast<const CAccessorFor_ACL*>(pFindData);
}

class CAccessControlList
{
public:
	std::vector<SPCAccessControlEntryBase> m_oAccessControlEntryList;

public:
	bool IsEffectivelyIdenticalTo( const CAccessControlList& oOther );

public:
	HRESULT LogData( const logging::CMessageContext& oMessageContext ) const;

protected:
	HRESULT Initialize( const ACL* pACL );

public:
	CAccessControlList() = default;
	CAccessControlList( const ACL* pACL )
	{
		Initialize( pACL );
	}
	CAccessControlList( const CAccessControlList& ) = default;
};
using SPCAccessControlList = std::shared_ptr<CAccessControlList>;

NAMESPACE_END //( structure )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
