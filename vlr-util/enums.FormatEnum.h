#pragma once

#include "config.h"

#include "formatpf.h"
#include "util.IsBitSet.h"

namespace vlr {

namespace enums {

template< typename TNumericType >
class CFormatEnumBase
{
public:
	[[nodiscard]]
	static inline auto FormatAsNumber(TNumericType nValue)
	{
		return formatpf(_T("%d"), static_cast<unsigned int>(nValue));
	}

	template< typename TTestValue, typename TAction >
	static inline HRESULT OnBitSet_DoAction(TNumericType nValue, TTestValue nBitValue, const TAction& fAction)
	{
		if (!util::IsBitSet(nValue, nBitValue))
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
	: public CFormatEnumBase<std::underlying_type_t<TEnum>>
{
private:
	using this_type = CFormatEnum<TEnum>;
	using base_type = CFormatEnumBase<std::underlying_type_t<TEnum>>;

public:
	[[nodiscard]]
	static inline auto FormatValue(TEnum eValue)
	{
		return base_type::FormatAsNumber(eValue);
	}
};

#define VLR_ON_CASE_RETURN_STRING_OF_VALUE( value ) case value: return _T(#value)

} // namespace enums

} // namespace vlr
