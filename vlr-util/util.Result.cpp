#include "pch.h"
#include "util.Result.h"

#include "formatpf.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

vlr::tstring CResult::ToString() const
{
	return vlr::formatpf( _T( "0x%08X" ), m_hrResult );
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
