#pragma once

#include <variant>

#include "config.h"
#include "AppOptionSourceInfo.h"
#include "util.Result.h"
#include "UtilMacros.Assertions.h"

namespace vlr {

// Note: isTypeInList adopted from: https://stackoverflow.com/questions/53738890/static-assert-that-a-type-is-among-a-stdvariants-accepted-types

template <typename T, template <typename...> class C, typename ... Ts>
constexpr auto isTypeInList(C<Ts...> const&)
-> std::disjunction<std::is_same<T, Ts>...>;

template <typename T, typename V>
static constexpr bool isTypeInList_v
= decltype(isTypeInList<T>(std::declval<V>()))::value;

class CAppOptionSpecifiedValue
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
		bool,
		std::vector<BYTE>,
		std::vector<vlr::tstring>
	>;
	using VCachedOptionValue = std::variant<
		std::monostate,
		std::string,
		std::wstring,
		int32_t,
		uint32_t,
		uint64_t,
		double,
		bool,
		std::vector<BYTE>,
		std::vector<vlr::tstring>
	>;

protected:
	CAppOptionSourceInfo m_oAppOptionSourceInfo;
	vlr::tstring m_sNativeOptionName;
	VNativeOptionValue m_vNativeOptionValue;

	vlr::tstring m_sNormalizedOptionName;
	VCachedOptionValue m_vCachedOptionValue;

public:
	SResult SetAppOptionName(vlr::tstring_view svNativeOptionName);

	inline decltype(auto) withSource(SEAppOptionSource eAppOptionSource)
	{
		m_oAppOptionSourceInfo = CAppOptionSourceInfo{ eAppOptionSource };
		return *this;
	}
	inline decltype(auto) withSourceInfo(const CAppOptionSourceInfo& oAppOptionSourceInfo)
	{
		m_oAppOptionSourceInfo = oAppOptionSourceInfo;
		return *this;
	}
	inline decltype(auto) withName(vlr::tstring_view svNativeOptionName)
	{
		SetAppOptionName(svNativeOptionName);
		return *this;
	}
	inline decltype(auto) withValue(const std::string_view& svValue)
	{
		m_vNativeOptionValue = std::string{ svValue };
		return *this;
	}
	inline decltype(auto) withValue(const std::wstring_view& svValue)
	{
		m_vNativeOptionValue = std::wstring{ svValue };
		return *this;
	}
	template <typename TValue, typename std::enable_if_t<isTypeInList_v<TValue, VNativeOptionValue>>* = nullptr>
	inline decltype(auto) withValue(const TValue& tValue)
	{
		m_vNativeOptionValue = tValue;
		return *this;
	}

	const auto& GetAppOptionSourceInfo() const
	{
		return m_oAppOptionSourceInfo;
	}
	const auto& GetNativeOptionName() const
	{
		return m_sNativeOptionName;
	}
	const auto& GetNativeOptionValue() const
	{
		return m_vNativeOptionValue;
	}
	const auto& GetNormalizedOptionName() const
	{
		return m_sNormalizedOptionName;
	}
	const auto& GetCachedOptionValue() const
	{
		return m_vCachedOptionValue;
	}

	static SResult PopulateOptionNameElements_DefaultDelimiters(
		const vlr::tstring& sOptionName,
		std::vector<vlr::tstring_view>& vecOptionNameElements);
	static SResult CheckOptionNameMatch(
		const std::vector<vlr::tstring_view>& vecOptionNameElements_1,
		const std::vector<vlr::tstring_view>& vecOptionNameElements_2);
	static SResult CheckOptionNameMatch(
		const vlr::tstring& sSpecifiedOptionName,
		const vlr::tstring& sNormalizedOptionName);

	// Returns:
	// - S_OK: option match (and populates normalized name)
	// - S_FALSE: not match
	SResult CheckForOptionMatch(
		const vlr::tstring& sNormalizedOptionName,
		const std::vector<vlr::tstring_view>& vecNormalizedOptionNameElements);
	inline SResult CheckForOptionMatch(
		const vlr::tstring& sNormalizedOptionName)
	{
		SResult sr;

		std::vector<vlr::tstring_view> vecOptionNameElements;
		sr = CAppOptionSpecifiedValue::PopulateOptionNameElements_DefaultDelimiters(sNormalizedOptionName, vecOptionNameElements);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

		return CheckForOptionMatch(sNormalizedOptionName, vecOptionNameElements);
	}

	SResult ConvertOptionValueTo(std::string& saValue) const;
	SResult ConvertOptionValueTo(std::wstring& swValue) const;
	SResult ConvertOptionValueTo(int32_t& nValue) const;
	SResult ConvertOptionValueTo(uint32_t& nValue) const;
	SResult ConvertOptionValueTo(uint64_t& nValue) const;
	SResult ConvertOptionValueTo(double& nValue) const;
	SResult ConvertOptionValueTo(bool& bValue) const;

	template <typename TValue>
	SResult ExtractOptionValueTo(TValue& tValue) const
	{
		// If we're asking for a type which cannot possibly be directly extracted, then always convert
		if constexpr (!isTypeInList_v<TValue, decltype(m_vNativeOptionValue)>)
		{
			return ConvertOptionValueTo(tValue);
		}
		else
		{
			auto pDirectValue = std::get_if<TValue>(&m_vNativeOptionValue);
			if (pDirectValue)
			{
				tValue = *pDirectValue;
				return S_OK;
			}

			return ConvertOptionValueTo(tValue);
		}
	}

	// This method converts the option value to the specified type, for quick inline extraction (without conversion).
	// The idea is that we reduce conversions on access, by caching the desired type during ingestion.
	template <typename TValue>
	SResult CacheOptionValueAs()
	{
		// This call is only valid if the cached value can hold the requested type
		static_assert(isTypeInList_v<TValue, decltype(m_vCachedOptionValue)>);

		auto pDirectValue = std::get_if<TValue>(&m_vNativeOptionValue);
		if (pDirectValue)
		{
			m_vCachedOptionValue = m_vNativeOptionValue;
			return S_OK;
		}

		TValue tValue{};
		auto sr = ConvertOptionValueTo(tValue);
		if (sr != S_OK)
		{
			return sr;
		}

		m_vCachedOptionValue = tValue;

		return S_OK;
	}

	template <typename TValue>
	SResult PopulateFromCachedValue(TValue& tValue)
	{
		auto pDirectValue = std::get_if<TValue>(&m_vCachedOptionValue);
		if (!pDirectValue)
		{
			return S_FALSE;
		}

		tValue = *pDirectValue;

		return S_OK;
	}

	template <typename TValue>
	const TValue& GetCachedValueInline_OrThrow() const noexcept(false)
	{
		auto pDirectValue = std::get_if<TValue>(&m_vCachedOptionValue);
		if (pDirectValue)
		{
			return *pDirectValue;
		}
		throw std::logic_error(fmt::format("Cached value for option '{}' is not the requested type", util::Convert::ToStdStringA(m_sNormalizedOptionName)));
	}

public:
	CAppOptionSpecifiedValue() = default;
	template <typename TValue>
	CAppOptionSpecifiedValue(
		const CAppOptionSourceInfo& oAppOptionSourceInfo,
		vlr::tstring_view svNativeOptionName,
		const TValue& tValue)
		: m_oAppOptionSourceInfo{ oAppOptionSourceInfo }
		, m_sNativeOptionName{ svNativeOptionName }
	{
		withValue(tValue);
	}
};
using SPCAppOptionSpecifiedValue = std::shared_ptr<CAppOptionSpecifiedValue>;

} // namespace vlr
