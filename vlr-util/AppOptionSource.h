#pragma once

#include "vlr-util/enums.SmartEnum.h"
#include "vlr-util/StringCompare.h"

namespace vlr {

namespace AppOptionSource {

enum EAppOptionSource
{
	Unknown,
	ExplicitViaCode,
	CommandLine,
	BinaryRelatedConfigFile,
	SystemConfigRespository,
};
constexpr auto MAX_VALUE = SystemConfigRespository;

} // namespace AppOptionSource

template<>
class vlr::enums::CFormatEnum<AppOptionSource::EAppOptionSource>
{
	using TEnum = AppOptionSource::EAppOptionSource;
	using this_type = CFormatEnum<TEnum>;
	using base_type = CFormatEnumBase<DWORD_PTR>;

public:
	[[nodiscard]]
	static inline auto FormatValue(AppOptionSource::EAppOptionSource eValue)
		-> vlr::tstring
	{
		switch (eValue)
		{
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(AppOptionSource::Unknown);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(AppOptionSource::ExplicitViaCode);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(AppOptionSource::CommandLine);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(AppOptionSource::BinaryRelatedConfigFile);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(AppOptionSource::SystemConfigRespository);

		default:
			return base_type::FormatAsNumber(eValue);
		}
	}
};

template<>
class vlr::enums::RangeCheckForEnum<AppOptionSource::EAppOptionSource>
	: CRangeInfoSequential_DWORD_PTR<AppOptionSource::MAX_VALUE>
{
	using TBaseEnum = AppOptionSource::EAppOptionSource;
	using ThisType = RangeCheckForEnum<TBaseEnum>;
	using TParentType = CRangeInfoSequential_DWORD_PTR<AppOptionSource::MAX_VALUE>;

public:
	using TParentType::IsValueInRange;
	static TBaseEnum CheckedEnumCast(DWORD_PTR dwValue)
	{
		if (IsValueInRange(dwValue))
		{
			return (TBaseEnum)dwValue;
		}
		else
		{
			return (TBaseEnum)0;
		}
	}
};

using SEAppOptionSource = vlr::enums::SmartEnum<
	AppOptionSource::EAppOptionSource,
	AppOptionSource::Unknown,
	vlr::enums::CFormatEnum<AppOptionSource::EAppOptionSource>,
	vlr::enums::RangeCheckForEnum<AppOptionSource::EAppOptionSource>
>;

} // namespace vlr
