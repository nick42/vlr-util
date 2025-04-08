#pragma once

#include "config.h"

#include "formatpf.h"
#include "util.IsBitSet.h"

#include "enums.FormatEnum.h"
#include "enums.RangeInfo.h"
#include "util.static_assert.h"
#include "util.convert.StringConversion.h"

namespace vlr {

namespace enums {

template<typename TBaseEnum>
class RangeCheckForEnum
{
	// For specialization, define:

	static bool IsValueInRange(DWORD_PTR dwValue)
	{
		VLR_TYPE_DEPENDENT_STATIC_FAIL(TBaseEnum, "Not implemented in generic base");
	}
	static TBaseEnum CheckedEnumCast(DWORD_PTR dwValue)
	{
		VLR_TYPE_DEPENDENT_STATIC_FAIL(TBaseEnum, "Not implemented in generic base");
	}
};

//template<typename TBaseEnum>
//constexpr bool isRangeCheckForEnumDefined()
//{
//	if constexpr (std::is_invocable_v<typename RangeCheckForEnum<TBaseEnum>::CheckedEnumCast, TBaseEnum>)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

template<typename TBaseEnum>
class RangeCheckForEnum_Selector
{
	using RangeCheckType = RangeCheckForEnum<TBaseEnum>;

public:
	static inline bool IsValueInRange(TBaseEnum eValue)
	{
		if constexpr (std::is_invocable_v<typename RangeCheckType::IsValueInRange, TBaseEnum>)
		{
			return RangeCheckType::IsValueInRange(eValue);
		}
		else
		{
			return true;
		}
	}
	static inline TBaseEnum CheckedEnumCast(DWORD_PTR dwValue)
	{
		if constexpr (std::is_invocable_v<typename RangeCheckType::CheckedEnumCast, TBaseEnum>)
		{
			return RangeCheckType::CheckedEnumCast(dwValue);
		}
		else
		{
			return (TBaseEnum)dwValue;
		}
	}
};

template<
	typename TBaseEnum,
	TBaseEnum tDefaultValue = (TBaseEnum)0,
	typename TEnumFormatter = CFormatEnum<TBaseEnum>,
	typename TRangeInfo = RangeCheckForEnum_Selector<TBaseEnum>
>
class SmartEnum
{
public:
	using ThisType = SmartEnum<TBaseEnum, tDefaultValue, TEnumFormatter, TRangeInfo>;
	using EnumFormatter = TEnumFormatter;
	using RangeInfo = TRangeInfo;
	// Class may be based on actual enum, or on numerical type (as wrapper for formatting, for example)
	static constexpr bool m_bBaseIsActualEnum = std::is_enum_v<TBaseEnum>;

protected:
	TBaseEnum m_eValue = tDefaultValue;

public:
	constexpr SmartEnum(TBaseEnum eValue = tDefaultValue) throw()
		: m_eValue{ eValue }
	{}
	// Only enable this alternative construction if initialized with a real enum type
	template <typename = std::enable_if_t<std::is_enum_v<TBaseEnum>>>
	// Warn on implicit conversion from numeric value to enum
	[[deprecated("SmartEnum should only be initialized with the underlying enum type to prevent possible errors.")]]
	constexpr SmartEnum(DWORD_PTR dwValue) throw()
		: m_eValue{ RangeInfo::CheckedEnumCast(dwValue) }
	{}

	// This should allow usage in switch statements
	constexpr operator TBaseEnum() const throw()
	{
		return m_eValue;
	}

	// Allow direct assignment IFF the underlying type is an actual enum
	template <typename = std::enable_if_t<m_bBaseIsActualEnum>>
	inline SmartEnum& operator=(TBaseEnum eValue) throw()
	{
		m_eValue = eValue;
		return *this;
	}
	// Allow conversion from number with range check in all cases
	inline SmartEnum& operator=(DWORD_PTR dwValue) throw()
	{
		m_eValue = RangeInfo::CheckedEnumCast(dwValue);
		return *this;
	}

	inline auto ToString() const
	{
		return EnumFormatter::FormatValue(m_eValue);
	}
	inline auto ToStringA() const
	{
		return vlr::util::Convert::ToStdStringA(ToString());
	}

	// Allow explicit creation from numerical value, as long as explicit call is made
	template <typename TValue, typename = std::enable_if_t<std::is_arithmetic_v<TValue>>>
	static constexpr auto FromNumber(TValue tValue) throw()
	{
		return SmartEnum{ RangeInfo::CheckedEnumCast(tValue) };
	}

	// TODO: Add FromString, using range iterators

};

} // namespace enums

} // namespace vlr
