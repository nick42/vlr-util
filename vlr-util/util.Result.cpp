#include "pch.h"
#include "util.Result.h"

#include "formatpf.h"

namespace vlr {

namespace util {

vlr::tstring SResult::ToString() const
{
	return vlr::formatpf(_T("0x%08X"), m_nResultCode);
}

} // namespace util

} // namespace vlr
