#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "formatpf.h"
#include "util.IsBitSet.h"

#include "enums.FormatEnum.h"
#include "enums.RangeInfo.h"
#include "util.static_assert.h"

namespace vlr {

namespace enums {

template<typename TBaseEnum>
class RangeCheckForEnum
{
	// For specialization, define:

	static bool IsValueInRange(DWORD_PTR dwValue)
	{
		VLR_STATIC_FAIL("Not implemented in generic base");
	}
	static TBaseEnum CheckedEnumCast(DWORD_PTR dwValue)
	{
		VLR_STATIC_FAIL("Not implemented in generic base");
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
		if constexpr (std::is_invocable_v<typename RangeCheckType::IsValueInRange, eValue>)
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
		if constexpr (std::is_invocable_v<typename RangeCheckType::CheckedEnumCast, dwValue>)
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
	using ThisType = SmartEnum< TBaseEnum, tDefaultValue, TEnumFormatter, TRangeInfo >;
	using EnumFormatter = TEnumFormatter;
	using RangeInfo = TRangeInfo;

protected:
	TBaseEnum m_eValue = tDefaultValue;

public:
	constexpr SmartEnum(TBaseEnum eValue = tDefaultValue) throw()
		: m_eValue{ eValue }
	{}
	[[deprecated("SmartEnum should only be initialized with the underlying enum type to prevent possible errors.")]]
	constexpr SmartEnum(DWORD_PTR dwValue) throw()
		: m_eValue{ RangeInfo::CheckedEnumCast(dwValue) }
	{}

	// This should allow usage in switch statements
	constexpr operator TBaseEnum() const throw()
	{
		return m_eValue;
	}

	inline SmartEnum& operator=(TBaseEnum eValue) throw()
	{
		m_eValue = eValue;
		return *this;
	}
	inline SmartEnum& operator=(DWORD_PTR dwValue) throw()
	{
		m_eValue = RangeInfo::CheckedEnumCast(dwValue);
		return *this;
	}

	inline auto ToString() const
	{
		return EnumFormatter::FormatValue(m_eValue);
	}
};

} // namespace enums

} // namespace vlr
