#include "pch.h"
#include "util.Result.h"

#include "formatpf.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( util )

vlr::tstring CResult::ToString() const
{
	return vlr::formatpf( _T( "0x%08X" ), m_hrResult );
}

VLR_NAMESPACE_END //( util )

VLR_NAMESPACE_END //( vlr )
