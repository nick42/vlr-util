#pragma once

#include "config.h"

#include "util.crc32.h"
#include "zstring_view.h"

namespace vlr {

class CResourceStringLookupValue
{
public:
	vlr::tzstring_view m_svzFormatValue;
	uint32_t m_nHashCRC{};

public:
	constexpr CResourceStringLookupValue(
		vlr::tzstring_view svzFormatValue)
		: m_svzFormatValue{ svzFormatValue }
		, m_nHashCRC{ util::crc32(svzFormatValue) }
	{}
};

} // namespace vlr
