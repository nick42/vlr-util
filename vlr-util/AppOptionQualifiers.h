#pragma once

#include <memory>

#include "config.h"

namespace vlr {

namespace AppOptionQualifiers {

namespace StandardFlags {

enum EStandardFlag
{
	IsSensitive = 1 << 0,
};

} // namespace StandardFlags

} // namespace AppOptionQualifiers

class CAppOptionQualifiers
{
public:
	uint32_t m_nFlags_Standard = 0;
	uint32_t m_nFlags_AppSpecific = 0;

	decltype(auto) withStandardFlagSet(uint32_t nFlag)
	{
		m_nFlags_Standard |= nFlag;
		return *this;
	}
	decltype(auto) withStandardFlagUnset(uint32_t nFlag)
	{
		m_nFlags_Standard &= (~nFlag);
		return *this;
	}
	decltype(auto) withAppSpecificFlagSet(uint32_t nFlag)
	{
		m_nFlags_AppSpecific |= nFlag;
		return *this;
	}
	decltype(auto) withAppSpecificFlagUnset(uint32_t nFlag)
	{
		m_nFlags_AppSpecific &= (~nFlag);
		return *this;
	}
};
using SPCAppOptionQualifiers = std::shared_ptr<CAppOptionQualifiers>;
using SPcCAppOptionQualifiers = std::shared_ptr<const CAppOptionQualifiers>;

} // namespace vlr
