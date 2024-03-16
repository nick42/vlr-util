#include "pch.h"
#include "AppOptionSpecifiedValue.h"

#include "vlr-util/strings.split.h"
#include "vlr-util/util.convert.StringConversion.h"
#include "vlr-util/util.overloaded.h"
#include "vlr-util/util.range_checked_cast.h"

namespace vlr {

SResult CAppOptionSpecifiedValue::SetAppOptionName(vlr::tzstring_view svzNativeOptionName)
{
	m_sNativeOptionName = svzNativeOptionName;

	return S_OK;
}

SResult CAppOptionSpecifiedValue::PopulateOptionNameElements_DefaultDelimiters(
	const vlr::tstring& sOptionName,
	std::vector<vlr::tstring_view>& vecOptionNameElements)
{
	SResult sr;

	static const auto oNameTreeDelimiters = strings::DelimitersSpec<TCHAR>{ std::vector<TCHAR>{ _T(':'), _T('.') } };

	sr = strings::SplitStringAtDelimiter<TCHAR>(sOptionName, oNameTreeDelimiters, vecOptionNameElements);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	return S_OK;
}

SResult CAppOptionSpecifiedValue::CheckForOptionMatch(
	const vlr::tstring& sNormalizedOptionName,
	const std::vector<vlr::tstring_view>& vecOptionNameElements)
{
	auto oStringCompare_OptionNameTree = StringCompare::CI();

	std::vector<vlr::tstring_view> vecNativeOptionNameElements;
	PopulateOptionNameElements_DefaultDelimiters(m_sNativeOptionName, vecNativeOptionNameElements);

	if (vecNativeOptionNameElements.size() != vecOptionNameElements.size())
	{
		return S_FALSE;
	}
	for (size_t i = 0; i < vecNativeOptionNameElements.size(); ++i)
	{
		if (!oStringCompare_OptionNameTree.AreEqual(vecNativeOptionNameElements[i], vecOptionNameElements[i]))
		{
			return S_FALSE;
		}
	}

	// Match to option

	m_sNormalizedOptionName = sNormalizedOptionName;

	return S_OK;
}

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(std::string& saValue) const
{
	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			saValue = tValue;
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			saValue = util::Convert::ToStdStringA(tValue);
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			saValue = std::to_string(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			saValue = std::to_string(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			saValue = std::to_string(tValue);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			saValue = std::to_string(tValue);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			saValue = std::to_string(tValue);
		}
		else if constexpr (std::is_same_v<TValue, std::vector<BYTE>>)
		{
			return S_FALSE;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<vlr::tstring>>)
		{
			return S_FALSE;
		}
		else
		{
			VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(S_FALSE);
		}

		// Note: This is unreachable code in the cases where we return early; suppress the warning
#pragma warning(push)
#pragma warning(disable: 4702)
		return S_OK;
	}, m_vNativeOptionValue);
#pragma warning(pop)
}

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(std::wstring& swValue) const
{
	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			swValue = util::Convert::ToStdStringW(tValue);
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			swValue = tValue;
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			swValue = std::to_wstring(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			swValue = std::to_wstring(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			swValue = std::to_wstring(tValue);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			swValue = std::to_wstring(tValue);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			swValue = std::to_wstring(tValue);
		}
		else if constexpr (std::is_same_v<TValue, std::vector<BYTE>>)
		{
			return S_FALSE;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<vlr::tstring>>)
		{
			return S_FALSE;
		}
		else
		{
			VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(S_FALSE);
		}

		// Note: This is unreachable code in the cases where we return early; suppress the warning
#pragma warning(push)
#pragma warning(disable: 4702)
		return S_OK;
	}, m_vNativeOptionValue);
#pragma warning(pop)
}

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(int32_t& nValue) const
{
	using TDest = int32_t;

	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(std::atoi(tValue.c_str()));
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(std::wcstol(tValue.c_str(), nullptr, 10));
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			nValue = tValue ? 1 : 0;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<BYTE>>)
		{
			return S_FALSE;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<vlr::tstring>>)
		{
			return S_FALSE;
		}
		else
		{
			VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(S_FALSE);
		}

		// Note: This is unreachable code in the cases where we return early; suppress the warning
#pragma warning(push)
#pragma warning(disable: 4702)
		return S_OK;
	}, m_vNativeOptionValue);
#pragma warning(pop)
}

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(uint32_t& nValue) const
{
	using TDest = uint32_t;

	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(std::stoul(tValue.c_str()));
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(std::wcstoul(tValue.c_str(), nullptr, 10));
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			nValue = tValue ? 1 : 0;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<BYTE>>)
		{
			return S_FALSE;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<vlr::tstring>>)
		{
			return S_FALSE;
		}
		else
		{
			VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(S_FALSE);
		}

		// Note: This is unreachable code in the cases where we return early; suppress the warning
#pragma warning(push)
#pragma warning(disable: 4702)
		return S_OK;
	}, m_vNativeOptionValue);
#pragma warning(pop)
}

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(uint64_t& nValue) const
{
	using TDest = uint64_t;

	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(std::stoull(tValue.c_str()));
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(std::wcstoull(tValue.c_str(), nullptr, 10));
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			nValue = vlr::util::range_checked_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			nValue = tValue ? 1 : 0;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<BYTE>>)
		{
			return S_FALSE;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<vlr::tstring>>)
		{
			return S_FALSE;
		}
		else
		{
			VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(S_FALSE);
		}

		// Note: This is unreachable code in the cases where we return early; suppress the warning
#pragma warning(push)
#pragma warning(disable: 4702)
		return S_OK;
	}, m_vNativeOptionValue);
#pragma warning(pop)
}

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(double& nValue) const
{
	using TDest = double;

	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			nValue = std::stod(tValue.c_str());
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			nValue = std::wcstod(tValue.c_str(), nullptr);
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			nValue = static_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			nValue = static_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			nValue = static_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			nValue = static_cast<TDest>(tValue);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			nValue = tValue ? 1 : 0;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<BYTE>>)
		{
			return S_FALSE;
		}
		else if constexpr (std::is_same_v<TValue, std::vector<vlr::tstring>>)
		{
			return S_FALSE;
		}
		else
		{
			VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION(S_FALSE);
		}

		// Note: This is unreachable code in the cases where we return early; suppress the warning
#pragma warning(push)
#pragma warning(disable: 4702)
		return S_OK;
	}, m_vNativeOptionValue);
#pragma warning(pop)
}

} // namespace vlr
