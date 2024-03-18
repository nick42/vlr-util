#include "pch.h"
#include "AppOptionSpecifiedValue.h"

#include <string>

#include "vlr-util/strings.split.h"
#include "vlr-util/util.convert.StringConversion.h"
#include "vlr-util/util.overloaded.h"
#include "vlr-util/util.range_checked_cast.h"

namespace vlr {

namespace {

// Note: The std:: conversion method throw exceptions on errors, so convert to result

template <typename TConversionCallable>
SResult ConvertStringToResult_StdLib(const TConversionCallable& fConversion)
{
	try
	{
		fConversion();
	}
	catch (const std::invalid_argument& /*ex*/)
	{
		return S_FALSE;
	}
	catch (const std::out_of_range& /*ex*/)
	{
		return S_FALSE;
	}

	return S_OK;
}

template <typename TBoundConverstionFunction, typename TResult>
SResult ConvertStringToNumber_StdLib(const TBoundConverstionFunction& fConverstionFunction, size_t nExpectedConversionLength, TResult& tResult)
{
	using TDest = TResult;

	std::size_t nSizeConverted{};
	return ConvertStringToResult_StdLib([&] {
		auto nValue = fConverstionFunction(&nSizeConverted);
		if (nSizeConverted != nExpectedConversionLength)
		{
			throw std::invalid_argument{"Failed conversion"};
		}
		tResult = vlr::util::range_checked_cast<TDest>(nValue);
	});
}

}

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

SResult CAppOptionSpecifiedValue::CheckOptionNameMatch(
	const std::vector<vlr::tstring_view>& vecOptionNameElements_1,
	const std::vector<vlr::tstring_view>& vecOptionNameElements_2)
{
	auto oStringCompare_OptionNameTree = StringCompare::CI();

	if (vecOptionNameElements_1.size() != vecOptionNameElements_2.size())
	{
		return S_FALSE;
	}
	for (size_t i = 0; i < vecOptionNameElements_1.size(); ++i)
	{
		if (!oStringCompare_OptionNameTree.AreEqual(vecOptionNameElements_1[i], vecOptionNameElements_2[i]))
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

SResult CAppOptionSpecifiedValue::CheckForOptionMatch(
	const vlr::tstring& sNormalizedOptionName,
	const std::vector<vlr::tstring_view>& vecNormalizedOptionNameElements)
{
	std::vector<vlr::tstring_view> vecNativeOptionNameElements;
	PopulateOptionNameElements_DefaultDelimiters(m_sNativeOptionName, vecNativeOptionNameElements);

	auto sr = CheckOptionNameMatch(vecNativeOptionNameElements, vecNormalizedOptionNameElements);
	VLR_ON_HR_NON_S_OK__RETURN_HRESULT(sr);

	// Match to option

	m_sNormalizedOptionName = sNormalizedOptionName;

	return S_OK;
}

SResult CAppOptionSpecifiedValue::CheckOptionNameMatch(
	const vlr::tstring& sSpecifiedOptionName,
	const vlr::tstring& sNormalizedOptionName)
{
	std::vector<vlr::tstring_view> vecSpecifiedOptionNameElements;
	PopulateOptionNameElements_DefaultDelimiters(sSpecifiedOptionName, vecSpecifiedOptionNameElements);

	std::vector<vlr::tstring_view> vecNormalizedOptionNameElements;
	PopulateOptionNameElements_DefaultDelimiters(sNormalizedOptionName, vecNormalizedOptionNameElements);

	return CheckOptionNameMatch(vecNormalizedOptionNameElements, vecNormalizedOptionNameElements);
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
			auto fConversionFunction = [&](auto pSize) {
				return std::stoi(tValue, pSize, 10);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			auto fConversionFunction = [&](auto pSize) {
				return std::stoi(tValue, pSize, 10);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
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
			auto fConversionFunction = [&](auto pSize) {
				return std::stoul(tValue, pSize, 10);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			auto fConversionFunction = [&](auto pSize) {
				return std::stoul(tValue, pSize, 10);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
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
			auto fConversionFunction = [&](auto pSize) {
				return std::stoull(tValue, pSize, 10);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			auto fConversionFunction = [&](auto pSize) {
				return std::stoull(tValue, pSize, 10);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
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
			auto fConversionFunction = [&](auto pSize) {
				return std::stod(tValue, pSize);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			auto fConversionFunction = [&](auto pSize) {
				return std::stod(tValue, pSize);
			};
			return ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
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

SResult CAppOptionSpecifiedValue::ConvertOptionValueTo(bool& bValue) const
{
	using TDest = bool;

	auto oStringCompare = StringCompare::CI();
	static constexpr auto svzTrue = tzstring_view{ _T("true") };
	static constexpr auto svzFalse = tzstring_view{ _T("false") };

	return std::visit([&](auto&& tValue) -> SResult {
		using TValue = std::decay_t<decltype(tValue)>;
		if constexpr (std::is_same_v<TValue, std::string>)
		{
			if (oStringCompare.AreEqual(tValue, svzTrue))
			{
				bValue = true;
			}
			else if (oStringCompare.AreEqual(tValue, svzFalse))
			{
				bValue = false;
			}
			else
			{
				auto fConversionFunction = [&](auto pSize) {
					return std::stol(tValue, pSize, 10);
				};
				long nValue{};
				auto sr = ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
				VLR_ON_HR_NON_S_OK__RETURN_HRESULT(sr);
				bValue = (nValue != 0);
			}
		}
		else if constexpr (std::is_same_v<TValue, std::wstring>)
		{
			if (oStringCompare.AreEqual(tValue, svzTrue))
			{
				bValue = true;
			}
			else if (oStringCompare.AreEqual(tValue, svzFalse))
			{
				bValue = false;
			}
			else
			{
				auto fConversionFunction = [&](auto pSize) {
					return std::stol(tValue, pSize, 10);
				};
				long nValue{};
				auto sr = ConvertStringToNumber_StdLib(fConversionFunction, tValue.length(), nValue);
				VLR_ON_HR_NON_S_OK__RETURN_HRESULT(sr);
				bValue = (nValue != 0);
			}
		}
		else if constexpr (std::is_same_v<TValue, int32_t>)
		{
			bValue = (tValue != 0);
		}
		else if constexpr (std::is_same_v<TValue, uint32_t>)
		{
			bValue = (tValue != 0);
		}
		else if constexpr (std::is_same_v<TValue, uint64_t>)
		{
			bValue = (tValue != 0);
		}
		else if constexpr (std::is_same_v<TValue, double>)
		{
			bValue = (tValue != 0);
		}
		else if constexpr (std::is_same_v<TValue, bool>)
		{
			bValue = tValue;
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
