#include "pch.h"
#include <vlr-util/AppOptions.h>

#include <gtest/gtest.h>

using namespace vlr;

TEST(AppOptions, AddSpecifiedValue)
{
	SResult sr;

	auto oAppOptions = CAppOptions{};

	auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
	spValue->withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue(42)
		;

	sr = oAppOptions.AddSpecifiedValue(spValue);
	EXPECT_EQ(sr, S_OK);

	sr = oAppOptions.AddSpecifiedValue(spValue);
	EXPECT_EQ(sr, SResult::Success_WithNuance);

	SPCAppOptionSpecifiedValue spValueInAppOptions;
	sr = oAppOptions.FindSpecifiedValueByName(spValue->GetNativeOptionName(), spValueInAppOptions);
	EXPECT_EQ(sr, S_OK);
	EXPECT_NE(spValueInAppOptions, nullptr);
	EXPECT_EQ(spValueInAppOptions.get(), spValue.get());
}

TEST(AppOptions, FindSpecifiedValuesMatchingNormalizedName)
{
	SResult sr;

	auto oAppOptions = CAppOptions{};

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("options.path.name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("options.Path.Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("SomethingElse")
			;

		sr = oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	std::vector<SPCAppOptionSpecifiedValue> vecSpecifiedValues;
	sr = oAppOptions.FindSpecifiedValuesMatchingNormalizedName(_T("Options::Path::Name"), vecSpecifiedValues);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(vecSpecifiedValues.size(), 3);
}

TEST(AppOptions, PopulateSpecifiedValueForOption)
{
	SResult sr;

	auto oAppOptions = CAppOptions{};

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("options.path.name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("SomethingElse")
			;

		sr = oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	SPCAppOptionSpecifiedValue spSpecifiedValue;
	sr = oAppOptions.PopulateSpecifiedValueForOption<uint32_t>(_T("Options::Path::Name"), spSpecifiedValue);
	EXPECT_EQ(sr, S_OK);
	EXPECT_NE(spSpecifiedValue, nullptr);

	uint32_t nValue{};
	EXPECT_NO_THROW(
		nValue = spSpecifiedValue->GetCachedValueInline_OrThrow<decltype(nValue)>();
	);
}

TEST(AppOptions, PopulatePrePreparedSpecifiedValueForOption)
{
	SResult sr;

	auto oAppOptions = CAppOptions{};

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = oAppOptions.PopulateSpecifiedValueForOption<uint32_t>(_T("Options::Path::Name"), spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_NE(spSpecifiedValue, nullptr);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = oAppOptions.PopulatePrePreparedSpecifiedValueForOption(_T("Options::Path::Name"), spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_NE(spSpecifiedValue, nullptr);

		uint32_t nValue{};
		EXPECT_NO_THROW(
			nValue = spSpecifiedValue->GetCachedValueInline_OrThrow<decltype(nValue)>();
		);
	}
}
