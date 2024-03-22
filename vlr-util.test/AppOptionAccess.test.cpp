#include "pch.h"
#include "vlr-util/AppOptionAccess.h"

#include <gtest/gtest.h>

using namespace vlr;

TEST(AppOptionAccess, GetValueOrDefault)
{
	SResult sr;

	static constexpr auto svzOptionName = _T("Options::AppOptionAccess::TheAnswer");

	auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };

	{
		auto nValue = oAppOption.GetValueOrDefault();
		EXPECT_EQ(nValue, 0);
		// Should also be able to directly compare, based on implicit operator
		EXPECT_EQ(oAppOption, 0);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(svzOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = CAppOptions::GetSharedInstance().AddSpecifiedValue(spValue);
		// Note: May be S_FALSE is previously set in another method
		EXPECT_TRUE(sr.isSuccess());
	}

	{
		auto nValue = oAppOption.GetValueOrDefault();
		EXPECT_EQ(nValue, 42);
		// Should also be able to directly compare, based on implicit operator
		EXPECT_EQ(oAppOption, 42);
	}
}

namespace Options::AppOptionAccess {

static constexpr auto GetNamespacePath() { return vlr::tzstring_view{ _T("Options::AppOptionAccess") }; }

VLR_DEFINE_APP_OPTION(DefinedAnswer, uint32_t, 42);
VLR_DEFINE_APP_OPTION(DefinedValue, uint32_t, 0);
VLR_DEFINE_APP_OPTION(DefinedValueNoSet, uint32_t, 0);

}

// Ensure we can get the default value in a constexpr context
struct TestConstexprDefaultValue
{
	void foo(uint32_t nValue = Options::AppOptionAccess::DefinedAnswer::GetDefaultValue())
	{}
};
static constexpr auto GetDefinedAnswerDefault()
{
	return Options::AppOptionAccess::DefinedAnswer::GetDefaultValue();
}

TEST(AppOptionAccess, GetValue_FromDefined_Default)
{
	SResult sr;

	{
		auto nValue = Options::AppOptionAccess::DefinedAnswer{}.GetValueOrDefault();
		EXPECT_EQ(nValue, 42);
	}
}

TEST(AppOptionAccess, GetValue_FromDefined_ExplicitSet)
{
	SResult sr;

	static constexpr auto svzOptionName = _T("Options::AppOptionAccess::DefinedValue");

	{
		auto nValue = Options::AppOptionAccess::DefinedValue{}.GetValueOrDefault();
		EXPECT_EQ(nValue, 0);
	}

	{
		auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };

		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(svzOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = CAppOptions::GetSharedInstance().AddSpecifiedValue(spValue);
		// Note: May be S_FALSE is previously set in another method
		EXPECT_TRUE(sr.isSuccess());
	}

	{
		auto nValue = Options::AppOptionAccess::DefinedValue{}.GetValueOrDefault();
		EXPECT_EQ(nValue, 42);
	}
}

TEST(AppOptionAccess, GetSpecifiedValuePtr)
{
	SResult sr;

	static constexpr auto svzOptionName = _T("Options::AppOptionAccess::DefinedValue");

	{
		auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };

		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(svzOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = CAppOptions::GetSharedInstance().AddSpecifiedValue(spValue);
		// Note: May be S_FALSE is previously set in another method
		EXPECT_TRUE(sr.isSuccess());
	}

	{
		auto pSpecifiedValue = Options::AppOptionAccess::DefinedValueNoSet{}.GetSpecifiedValuePtr();
		EXPECT_EQ(pSpecifiedValue, nullptr);
	}

	{
		auto pSpecifiedValue = Options::AppOptionAccess::DefinedValue{}.GetSpecifiedValuePtr();
		EXPECT_NE(pSpecifiedValue, nullptr);
		EXPECT_EQ(*pSpecifiedValue, 42);
	}
}
