#pragma once

#include <memory>

#include "config.h"

namespace vlr {

namespace AppOptionQualifiers {

namespace StandardFlags {

enum EStandardFlag
{
	// Intent: Affect handling of the option (eg: don't log in list of options)
	IsSensitive = 1 << 0,
	// Intent: Can be used to conditionally block ability to change value (eg: in a Release build)
	ReturnOnlyDefaultValue = 1 << 1,
};

} // namespace StandardFlags

} // namespace AppOptionQualifiers

class CAppOptionQualifiers
{
protected:
	uint32_t m_nFlags_Standard = 0;
	uint32_t m_nFlags_AppSpecific = 0;

public:
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

	inline auto& GetFlags_Standard() const
	{
		return m_nFlags_Standard;
	}
	inline auto& GetFlags_AppSpecific() const
	{
		return m_nFlags_AppSpecific;
	}

};
using SPCAppOptionQualifiers = std::shared_ptr<CAppOptionQualifiers>;
using SPcCAppOptionQualifiers = std::shared_ptr<const CAppOptionQualifiers>;

} // namespace vlr
