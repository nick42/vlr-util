#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

namespace vlr {

namespace detail {

template<typename T>
struct null_deleter
{
	inline void operator()(T* /*pValue*/) {}
};

} // namespace detail

template<typename T>
constexpr auto MakeNonOwningSharedPtr(T* pInstance)
{
	return std::shared_ptr<T>{ pInstance, detail::null_deleter<T>{} };
}
template<typename T>
constexpr auto MakeNonOwningSharedPtr(const T* pInstance)
{
	return std::shared_ptr<const T>{ pInstance, detail::null_deleter<T>{} };
}

} // namespace vlr
