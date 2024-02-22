#pragma once

#include <variant>

#include "config.h"
#include "AppOptionSource.h"
#include "util.Result.h"

namespace vlr {

class CAppOptionValue
{
public:
	using VNativeOptionValue = std::variant<
		std::monostate,
		std::string,
		std::wstring,
		int32_t,
		uint32_t,
		uint64_t,
		double,
		std::vector<BYTE>,
		std::vector<vlr::tstring>
	>;

public:
	SEAppOptionSource m_eAppOptionSource;
	vlr::tstring m_sNativeOptionName;
	VNativeOptionValue m_vNativeOptionValue;

	vlr::tstring m_sNormalizedOptionName;

public:
	SResult PopulateValueAs(std::string& sValue);
	// TODO: others...

	// Returns:
	// - S_OK: option match (and populates normalized name)
	// - S_FALSE: not match
	SResult CheckForOptionMatch(
		const vlr::tstring& sNormalizedOptionName,
		const std::vector<vlr::tstring_view>& vecOptionNameElements);

};

} // namespace vlr
