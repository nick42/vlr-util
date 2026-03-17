#pragma once

#include "AppOptions.h"
#include "AppOptionSourceInfo.h"
#include "util.IsBitSet.h"

namespace vlr {

template <typename TValue>
class CAppOptionAccess
{
protected:
	vlr::tstring m_sNormalizedOptionName;
	TValue m_tValue_Default{};

public:
	// Returns:
	// - If value specified, a pointer to the value
	// - If no value specified, nullptr
	const TValue* GetSpecifiedValuePtr() const;
	const TValue& GetValueOrDefault() const;

	inline bool HasSpecifiedValue() const
	{
		return (GetSpecifiedValuePtr() != nullptr);
	}

	inline operator const TValue& () const
	{
		return GetValueOrDefault();
	}

	SResult SetSpecifiedValue(
		const TValue& tValue,
		const CAppOptionSourceInfo& oAppOptionSourceInfo = CAppOptionSourceInfo::ExplicitViaCode()) const;

	SResult EnsureMetadataInStore(
		vlr::tzstring_view svzMetadata) const;

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
const TValue* CAppOptionAccess<TValue>::GetSpecifiedValuePtr() const
{
	SResult sr;

	auto& oAppOptions = CAppOptions::GetSharedInstance();

	SPCAppOptionSpecifiedValue spAppOptionSpecifiedValue;
	sr = oAppOptions.PopulateSpecifiedValueForOption<TValue>(m_sNormalizedOptionName, spAppOptionSpecifiedValue);
	if (sr != S_OK)
	{
		return nullptr;
	}
	if (!spAppOptionSpecifiedValue)
	{
		// No value specified for this option
		return nullptr;
	}

	const auto& oValue = spAppOptionSpecifiedValue->GetCachedValueInline_OrThrow<TValue>();
	return &oValue;
}

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
	if (spAppOptionSpecifiedValue->GetAppOptionQualifiers()
		&& util::IsBitSet(spAppOptionSpecifiedValue->GetAppOptionQualifiers()->GetFlags_Standard(), AppOptionQualifiers::StandardFlags::ReturnOnlyDefaultValue))
	{
		// Qualifier set to ignore specified value(s)
		return m_tValue_Default;
	}

	return spAppOptionSpecifiedValue->GetCachedValueInline_OrThrow<TValue>();
}

template <typename TValue>
SResult CAppOptionAccess<TValue>::SetSpecifiedValue(
	const TValue& tValue,
	const CAppOptionSourceInfo& oAppOptionSourceInfo /*= CAppOptionSourceInfo::ExplicitViaCode()*/) const
{
	SResult sr;

	auto& oAppOptions = CAppOptions::GetSharedInstance();

	auto spAppOptionSpecifiedValue = std::make_shared<CAppOptionSpecifiedValue>(
		oAppOptionSourceInfo,
		m_sNormalizedOptionName,
		tValue);

	return oAppOptions.AddSpecifiedValue(spAppOptionSpecifiedValue);
}

template <typename TValue>
SResult CAppOptionAccess<TValue>::EnsureMetadataInStore(
	vlr::tzstring_view svzMetadata) const
{
	auto& oAppOptions = CAppOptions::GetSharedInstance();

	return oAppOptions.SetAppOptionMetadata(m_sNormalizedOptionName, svzMetadata);
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

// Note: This alternative construction/macro exists to allow metadata in AppOption definitions. It's less efficient 
// than the version without metadata, since it requires an extra call to the options store to ensure the metadata is 
// present, but this is only at construction time, and allows metadata to be associated with options without needing 
// to maintain a separate collection of option metadata in the code.

#define VLR_DEFINE_APP_OPTION_WITH_METADATA(StructName, ValueType, DefaultValue, sMetadata) \
struct StructName \
	: public ::vlr::CAppOptionAccess<ValueType> \
{ \
public: \
	static constexpr decltype(auto) GetDefaultValue() { return (DefaultValue); } \
	static auto GetOptionName() { return fmt::format(_T("{}::{}"), GetNamespacePath(), _T(#StructName)); } \
	static auto GetMetadata() { return (sMetadata); } \
\
	/*template <typename TDefaultValue, typename std::enable_if_t<std::is_convertible_v<TDefaultValue, ValueType>>* = nullptr>*/ \
	StructName() \
		: ::vlr::CAppOptionAccess<ValueType>{ GetOptionName(), (DefaultValue) } \
	{ \
		EnsureMetadataInStore(GetMetadata()); \
	} \
};

} // namespace vlr
