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
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(svzOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = CAppOptions::GetSharedInstance().AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto nValue = oAppOption.GetValueOrDefault();
		EXPECT_EQ(nValue, 42);
	}
}

namespace Options::AppOptionAccess {

static constexpr auto GetNamespacePath() { return vlr::tzstring_view{ _T("Options::AppOptionAccess") }; }

VLR_DEFINE_APP_OPTION(DefinedAnswer, uint32_t, 42);
VLR_DEFINE_APP_OPTION(DefinedValue, uint32_t, 0);

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
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto nValue = Options::AppOptionAccess::DefinedValue{}.GetValueOrDefault();
		EXPECT_EQ(nValue, 42);
	}
}
