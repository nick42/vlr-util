#pragma once

#include "config.h"
#include "cpp_namespace.h"

namespace vlr {

// Note: span should always be const by default, so make that the default

template< typename TElement, typename TBase = cpp::span<const std::decay_t<TElement>> >
class as_span
	: public TBase
{
public:
	using TBase::TBase;
	template< typename TArrayElement >
	as_span( const std::vector<TArrayElement>& tArray )
		: TBase{ (TElement*)tArray.data(), tArray.size() * sizeof( TArrayElement ) / sizeof( TElement ) }
	{}
	template< typename TArrayElement >
	as_span( const std::basic_string<TArrayElement>& tArray )
		: TBase{ (TElement*)tArray.data(), tArray.size() * sizeof( TArrayElement ) / sizeof( TElement ) }
	{}
};

} // namespace vlr
