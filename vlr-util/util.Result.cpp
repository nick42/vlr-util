#include "pch.h"
#include "vlr/util.Result.h"

#include "vlr/formatpf.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

cpp::tstring CResult::ToString() const
{
	return vlr::formatpf( _T( "0x%08X" ), m_hrResult );
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
