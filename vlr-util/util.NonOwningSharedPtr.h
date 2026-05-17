#pragma once

#include "config.h"

// Note: This is a utility for creating non-owning shared_ptrs, which can be useful in some cases 
// (e.g. when you want to share an object across multiple components, but don't want to transfer ownership 
// or require the use of a shared_ptr for the original object, or for compat with methods expecting a shared_ptr).

namespace vlr {

namespace detail {

template <typename T>
struct null_deleter
{
	inline void operator()(T* /*pValue*/) {}
};

} // namespace detail

template <typename T>
[[nodiscard]]
constexpr auto MakeNonOwningSharedPtr_Caution(T* pInstance)
{
	return std::shared_ptr<T>{ pInstance, detail::null_deleter<T>{} };
}
template <typename T>
[[nodiscard]]
constexpr auto MakeNonOwningSharedPtr_Caution(const T* pInstance)
{
	return std::shared_ptr<const T>{ pInstance, detail::null_deleter<T>{} };
}

} // namespace vlr
