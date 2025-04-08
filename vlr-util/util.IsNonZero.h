#pragma once

#include "config.h"

#include "util.choice.h"

namespace vlr {

namespace util {

namespace detail {

template<typename From, typename To, typename = void>
struct is_narrowing_conversion_impl : std::true_type {};

template<typename From, typename To>
struct is_narrowing_conversion_impl < From, To, std::void_t<decltype(To{ std::declval<From>() }) >> : std::false_type {};

template<typename From, typename To>
struct is_narrowing_conversion : is_narrowing_conversion_impl<From, To> {};

template< typename TValue >
constexpr auto IsNonZero_choice( const TValue& tValue, choice<0>&& )
-> std::enable_if_t<std::is_convertible_v<TValue, bool> && !is_narrowing_conversion<TValue, bool>::value, bool>
{
	return !!(tValue);
}

template< typename TValue >
constexpr auto IsNonZero_choice( const TValue& tValue, choice<1>&& )
-> decltype(tValue != 0)
{
	return (tValue != 0);
}

template< typename TValue >
constexpr auto IsNonZero_choice( const TValue* tValue, choice<2>&& )
-> decltype(tValue != nullptr)
{
	return (tValue != nullptr);
}

} // namespace detail

template< typename TValue >
constexpr bool IsNonZero( const TValue& tValue )
{
	return detail::IsNonZero_choice( tValue, choice<0>{} );
}

} // namespace util

} // namespace vlr
