#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "formatpf.h"
#include "util.IsBitSet.h"

VLR_NAMESPACE_BEGIN(vlr)

VLR_NAMESPACE_BEGIN(enums)

template< typename TNumericType >
class CFormatEnumBase
{
public:
	[[nodiscard]]
	static inline auto FormatAsNumber( TNumericType nValue )
	{
		return formatpf( _T( "%d" ), static_cast<unsigned int>(nValue) );
	}

	template< typename TTestValue, typename TAction >
	static inline HRESULT OnBitSet_DoAction( TNumericType nValue, TTestValue nBitValue, const TAction& fAction )
	{
		if (!util.IsBitSet( nValue, nBitValue ))
		{
			return S_FALSE;
		}

		fAction();

		return S_OK;
	}
};

// Note: The idea here is to specialize this class on a per-enum basis, to provide better string conversion.

template< typename TEnum, typename std::enable_if_t<std::is_enum_v<TEnum>>* = nullptr >
class CFormatEnum
	: public CFormatEnumBase<DWORD_PTR>
{
private:
	using this_type = CFormatEnum<TEnum>;
	using base_type = CFormatEnumBase<DWORD_PTR>;

public:
	[[nodiscard]]
	static inline auto FormatValue( TEnum eValue )
	{
		return base_type::FormatAsNumber( eValue );
	}
};

#define VLR_ON_CASE_RETURN_STRING_OF_VALUE( value ) case value: return _T(#value)

VLR_NAMESPACE_END //(enums)

VLR_NAMESPACE_END //(vlr)
