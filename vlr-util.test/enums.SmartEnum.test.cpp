#include "pch.h"

#include "vlr-util/enums.SmartEnum.h"
#include "vlr-util/StringCompare.h"

namespace Color {

enum EColor
{
	Unknown,
	Red,
	Green,
	Blue,
};
constexpr auto MAX_VALUE = Blue;

} // namespace Color

template<>
class vlr::enums::CFormatEnum<Color::EColor>
{
	using TEnum = Color::EColor;
	using this_type = CFormatEnum<TEnum>;
	using base_type = CFormatEnumBase<DWORD_PTR>;

public:
	[[nodiscard]]
	static inline auto FormatValue(Color::EColor eValue)
		-> vlr::tstring
	{
		switch (eValue)
		{
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(Color::Unknown);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(Color::Red);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(Color::Green);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(Color::Blue);

		default:
			return base_type::FormatAsNumber(eValue);
		}
	}
};

template<>
class vlr::enums::RangeCheckForEnum<Color::EColor>
	: CRangeInfoSequential_DWORD_PTR<Color::MAX_VALUE>
{
	using TBaseEnum = Color::EColor;
	using ThisType = RangeCheckForEnum<TBaseEnum>;
	using TParentType = CRangeInfoSequential_DWORD_PTR<Color::MAX_VALUE>;

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

using SEColor = vlr::enums::SmartEnum<
	Color::EColor,
	Color::Unknown,
	vlr::enums::CFormatEnum<Color::EColor>,
	vlr::enums::RangeCheckForEnum<Color::EColor>
>;

TEST(SmartEnum, general)
{
	static_assert(sizeof(SEColor) == sizeof(Color::EColor));

	SEColor eColor;
	EXPECT_EQ(eColor, Color::Unknown);

	// Can set by default enum value
	eColor = Color::Red;
	EXPECT_EQ(eColor, Color::Red);

	// Can also set by enum index
	eColor = 1;
	EXPECT_EQ(eColor, Color::Red);

	// Setting values out of range should result in the default value
	eColor = 42;
	EXPECT_EQ(eColor, Color::Unknown);

	// Format value should have the enum value name in it (may be qualified)
	eColor = Color::Red;
	auto sValue = eColor.ToString();
	EXPECT_EQ(vlr::StringCompare::CS().StringHasPostfix(sValue, "Red"), true);

	eColor = SEColor::FromNumber((int)Color::Red);
	EXPECT_EQ(eColor, Color::Red);
}
