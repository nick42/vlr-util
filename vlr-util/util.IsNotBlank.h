#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.choice.h"
#include "zstring_view.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(util)

VLR_NAMESPACE_BEGIN(detail)

// Note: Using our type, to allow more implicit construction from string classes

template< typename TValue, typename std::enable_if_t<std::is_constructible_v<vlr::zstring_view, const TValue&>>* = nullptr >
constexpr bool IsNotBlank_choice(const TValue& tValue, choice<0>&&)
{
	return (vlr::zstring_view{ tValue }.length() > 0);
}

template< typename TValue, typename std::enable_if_t<std::is_constructible_v<vlr::wzstring_view, const TValue&>>* = nullptr >
constexpr bool IsNotBlank_choice(const TValue& tValue, choice<1>&&)
{
	return (vlr::wzstring_view{ tValue }.length() > 0);
}

//#ifdef VLR_CONFIG_INCLUDE_AFX
//
//template< typename TValue, typename std::enable_if_t<std::is_convertible_v<CStringA, const TValue&>>* = nullptr >
//constexpr bool IsNotBlank_choice(const TValue& tValue, choice<2>&&)
//{
//	return (CStringA{ tValue }.GetLength() > 0);
//}
//
//template< typename TValue, typename std::enable_if_t<std::is_convertible_v<CStringW, const TValue&>>* = nullptr >
//constexpr bool IsNotBlank_choice(const TValue& tValue, choice<3>&&)
//{
//	return (CStringW{ tValue }.GetLength() > 0);
//}
//
//#endif

//template< typename TValue, typename TFallbackChoice >
//constexpr bool IsNotBlank(const TValue& tValue, TFallbackChoice&&)
//{
//	return (tValue != "");
//}

VLR_NAMESPACE_END //(detail)

constexpr bool IsNotBlank(vlr::zstring_view svzValue)
{
	return (svzValue.length() > 0);
}

constexpr bool IsNotBlank(vlr::wzstring_view svzValue)
{
	return (svzValue.length() > 0);
}

constexpr bool IsNotBlank(std::string_view svValue)
{
	return (svValue.length() > 0);
}

constexpr bool IsNotBlank(std::wstring_view svValue)
{
	return (svValue.length() > 0);
}

template< typename TValue >
constexpr bool IsNotBlank(const TValue& tValue)
{
	return detail::IsNotBlank_choice(tValue, choice<0>{});
}

VLR_NAMESPACE_END //(util)

VLR_NAMESPACE_END //(vlr)
