#include "pch.h"
#include <vlr-util/AppOptions.h>

#include <gtest/gtest.h>

using namespace vlr;

class AppOptions
	: public testing::Test
{
protected:
	CAppOptions m_oAppOptions;
};

TEST_F(AppOptions, AddSpecifiedValue)
{
	SResult sr;

	auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
	spValue->withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue(42)
		;

	sr = m_oAppOptions.AddSpecifiedValue(spValue);
	EXPECT_EQ(sr, S_OK);

	sr = m_oAppOptions.AddSpecifiedValue(spValue);
	EXPECT_EQ(sr, SResult::Success_WithNuance);

	SPCAppOptionSpecifiedValue spValueInAppOptions;
	sr = m_oAppOptions.FindSpecifiedValueByName(spValue->GetNativeOptionName(), spValueInAppOptions);
	EXPECT_EQ(sr, S_OK);
	EXPECT_NE(spValueInAppOptions, nullptr);
	EXPECT_EQ(spValueInAppOptions.get(), spValue.get());
}

TEST_F(AppOptions, FindSpecifiedValuesMatchingNormalizedName)
{
	SResult sr;

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("options.path.name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(_T("options.Path.Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("SomethingElse")
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	std::vector<SPCAppOptionSpecifiedValue> vecSpecifiedValues;
	sr = m_oAppOptions.FindSpecifiedValuesMatchingNormalizedName(_T("Options::Path::Name"), vecSpecifiedValues);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(vecSpecifiedValues.size(), 3);
}

TEST_F(AppOptions, PopulateSpecifiedValueForOption)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Path::Name");
	static const vlr::tstring sAppOptionName_Alternative = _T("options.path.name");

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sAppOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sAppOptionName_Alternative)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("SomethingElse")
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	SPCAppOptionSpecifiedValue spSpecifiedValue;
	sr = m_oAppOptions.PopulateSpecifiedValueForOption<uint32_t>(sAppOptionName, spSpecifiedValue);
	EXPECT_EQ(sr, S_OK);
	EXPECT_NE(spSpecifiedValue, nullptr);

	uint32_t nValue{};
	EXPECT_NO_THROW(
		nValue = spSpecifiedValue->GetCachedValueInline_OrThrow<decltype(nValue)>();
	);
}

TEST_F(AppOptions, PopulatePrePreparedSpecifiedValueForOption)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Path::Name");

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sAppOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = m_oAppOptions.PopulateSpecifiedValueForOption<uint32_t>(sAppOptionName, spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_NE(spSpecifiedValue, nullptr);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = m_oAppOptions.PopulatePrePreparedSpecifiedValueForOption(sAppOptionName, spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_NE(spSpecifiedValue, nullptr);

		uint32_t nValue{};
		EXPECT_NO_THROW(
			nValue = spSpecifiedValue->GetCachedValueInline_OrThrow<decltype(nValue)>();
		);
	}
}

TEST_F(AppOptions, OptionWithQualifiers_BeforeSpecifiedValue)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Path::Name");

	{
		auto spQualifiers = std::make_shared<CAppOptionQualifiers>();
		spQualifiers->withStandardFlagSet(AppOptionQualifiers::StandardFlags::IsSensitive);

		m_oAppOptions.SetAppOptionQualifiers(sAppOptionName, spQualifiers);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sAppOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = m_oAppOptions.PopulateSpecifiedValueForOption<uint32_t>(sAppOptionName, spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		ASSERT_NE(spSpecifiedValue, nullptr);

		ASSERT_NE(spSpecifiedValue->GetAppOptionQualifiers(), nullptr);
		EXPECT_EQ(spSpecifiedValue->GetAppOptionQualifiers()->GetFlags_Standard(), AppOptionQualifiers::StandardFlags::IsSensitive);
	}
}

TEST_F(AppOptions, OptionWithQualifiers_AfterSpecifiedValue)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Path::Name");

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sAppOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(42)
			;

		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spQualifiers = std::make_shared<CAppOptionQualifiers>();
		spQualifiers->withStandardFlagSet(AppOptionQualifiers::StandardFlags::IsSensitive);

		m_oAppOptions.SetAppOptionQualifiers(sAppOptionName, spQualifiers);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = m_oAppOptions.PopulateSpecifiedValueForOption<uint32_t>(sAppOptionName, spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		ASSERT_NE(spSpecifiedValue, nullptr);

		ASSERT_NE(spSpecifiedValue->GetAppOptionQualifiers(), nullptr);
		EXPECT_EQ(spSpecifiedValue->GetAppOptionQualifiers()->GetFlags_Standard(), AppOptionQualifiers::StandardFlags::IsSensitive);
	}
}
