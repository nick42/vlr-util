#pragma once

#include "config.h"
#include "AppOptionSource.h"

namespace vlr {

class CAppOptionSourceInfo
{
public:
	SEAppOptionSource m_eAppOptionSource;
	vlr::tstring m_sSourceIndentifier;

	inline auto& withAppOptionSourceInfo(
		SEAppOptionSource eAppOptionSource,
		vlr::tzstring_view svzSourceIdentifier = {})
	{
		m_eAppOptionSource = eAppOptionSource;
		m_sSourceIndentifier = svzSourceIdentifier.toStdString();
		return *this;
	}

	static inline auto ExplicitViaCode()
	{
		return CAppOptionSourceInfo{}.withAppOptionSourceInfo(AppOptionSource::ExplicitViaCode);
	}

	CAppOptionSourceInfo() = default;
	CAppOptionSourceInfo(
		SEAppOptionSource eAppOptionSource,
		vlr::tzstring_view svzSourceIdentifier = {})
		: m_eAppOptionSource{ eAppOptionSource }
		, m_sSourceIndentifier{ svzSourceIdentifier.toStdString() }
	{}
};

} // namespace vlr
