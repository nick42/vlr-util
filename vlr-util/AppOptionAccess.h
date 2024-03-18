#pragma once

#include "AppOptions.h"

namespace vlr {

template <typename TValue>
class CAppOptionAccess
{
protected:
	vlr::tstring m_sNormalizedOptionName;
	TValue m_tValue_Default{};

public:
	const TValue& GetValueOrDefault() const;

public:
	CAppOptionAccess(
		vlr::tzstring_view svzNormalizedOptionName,
		const TValue& tValue_Default)
		: m_sNormalizedOptionName{ svzNormalizedOptionName.toStdString() }
		, m_tValue_Default{ tValue_Default }
	{}
	// Note: Anticipating normal use-case, with prvalue instances for params
	CAppOptionAccess(
		vlr::tzstring_view svzNormalizedOptionName,
		TValue&& tValue_Default)
		: m_sNormalizedOptionName{ svzNormalizedOptionName.toStdString() }
		, m_tValue_Default{ std::move(tValue_Default) }
	{}
};

template <typename TValue>
const TValue& CAppOptionAccess<TValue>::GetValueOrDefault() const
{
	SResult sr;

	auto& oAppOptions = CAppOptions::GetSharedInstance();

	SPCAppOptionSpecifiedValue spAppOptionSpecifiedValue;
	sr = oAppOptions.PopulateSpecifiedValueForOption<TValue>(m_sNormalizedOptionName, spAppOptionSpecifiedValue);
	if (sr != S_OK)
	{
		return m_tValue_Default;
	}
	if (!spAppOptionSpecifiedValue)
	{
		// No value specified for this option
		return m_tValue_Default;
	}

	return spAppOptionSpecifiedValue->GetCachedValueInline_OrThrow<TValue>();
}

// Note: This should compile IFF DefaultValue is convertable to ValueType

#define VLR_DEFINE_APP_OPTION(StructName, ValueType, DefaultValue) \
struct StructName \
	: public ::vlr::CAppOptionAccess<ValueType> \
{ \
public: \
	static constexpr decltype(auto) GetDefaultValue() { return (DefaultValue); } \
	static auto GetOptionName() { return fmt::format(_T("{}::{}"), GetNamespacePath(), _T(#StructName)); } \
\
	/*template <typename TDefaultValue, typename std::enable_if_t<std::is_convertible_v<TDefaultValue, ValueType>>* = nullptr>*/ \
	StructName() \
		: ::vlr::CAppOptionAccess<ValueType>{ GetOptionName(), (DefaultValue) } \
	{} \
};

} // namespace vlr
