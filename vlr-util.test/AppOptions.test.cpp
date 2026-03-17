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

TEST_F(AppOptions, ClearSpecifiedValue)
{
	SResult sr;

	static const vlr::tstring sName = _T("Options::Clear::Test");

	auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
	spValue->withName(sName)
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue(123)
		;

	// Add and verify present
	sr = m_oAppOptions.AddSpecifiedValue(spValue);
	EXPECT_EQ(sr, S_OK);

	SPCAppOptionSpecifiedValue spFound;
	sr = m_oAppOptions.FindSpecifiedValueByName(spValue->GetNativeOptionName(), spFound);
	EXPECT_EQ(sr, S_OK);
	EXPECT_NE(spFound, nullptr);

	// Clear the specified value and verify removed
	sr = m_oAppOptions.ClearSpecifiedValue(spValue);
	EXPECT_EQ(sr, SResult::Success);

	sr = m_oAppOptions.FindSpecifiedValueByName(spValue->GetNativeOptionName(), spFound);
	EXPECT_EQ(sr, S_FALSE);

	// Clearing again should indicate nuance (was not present)
	sr = m_oAppOptions.ClearSpecifiedValue(spValue);
	EXPECT_EQ(sr, SResult::Success_WithNuance);
}

TEST_F(AppOptions, ClearAllSpecifiedValues)
{
	SResult sr;

	static const vlr::tstring sNameA = _T("Options::ToClear::Name");
	static const vlr::tstring sNameB = _T("options.toclear.name");

	// Add two values that normalize to the same name
	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sNameA)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(1)
			;
		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sNameB)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(2)
			;
		sr = m_oAppOptions.AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	// Verify they are found via normalized lookup
	std::vector<SPCAppOptionSpecifiedValue> vec;
	sr = m_oAppOptions.FindSpecifiedValuesMatchingNormalizedName(_T("Options::ToClear::Name"), vec);
	EXPECT_EQ(sr, S_OK);
	EXPECT_GE(vec.size(), 2U);

	// Clear all and verify removal
	sr = m_oAppOptions.ClearAllSpecifiedValues();
	EXPECT_EQ(sr, SResult::Success);

	// Using normalized lookup to ensure no leftovers
	std::vector<SPCAppOptionSpecifiedValue> vecAfter;
	sr = m_oAppOptions.FindSpecifiedValuesMatchingNormalizedName(_T("Options::ToClear::Name"), vecAfter);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(vecAfter.size(), 0U);
}

TEST_F(AppOptions, SetAndPopulateAppOptionMetadata)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Metadata::Test");
	static const vlr::tstring sMetadataValue = _T("TestMetadata");

	// Set metadata
	sr = m_oAppOptions.SetAppOptionMetadata(sAppOptionName, sMetadataValue);
	EXPECT_EQ(sr, S_OK);

	// Populate and verify
	vlr::tstring sRetrievedMetadata;
	sr = m_oAppOptions.PopulateAppOptionMetadata(sAppOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedMetadata, sMetadataValue);
}

TEST_F(AppOptions, SetAppOptionMetadata_UpdateExisting)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Metadata::Update");
	static const vlr::tstring sMetadataValue1 = _T("FirstValue");
	static const vlr::tstring sMetadataValue2 = _T("UpdatedValue");

	// Set initial metadata
	sr = m_oAppOptions.SetAppOptionMetadata(sAppOptionName, sMetadataValue1);
	EXPECT_EQ(sr, S_OK);

	// Update metadata
	sr = m_oAppOptions.SetAppOptionMetadata(sAppOptionName, sMetadataValue2);
	EXPECT_EQ(sr, S_OK);

	// Verify updated value
	vlr::tstring sRetrievedMetadata;
	sr = m_oAppOptions.PopulateAppOptionMetadata(sAppOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedMetadata, sMetadataValue2);
}

TEST_F(AppOptions, PopulateAppOptionMetadata_NotSet)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Metadata::NotSet");

	vlr::tstring sRetrievedMetadata;
	sr = m_oAppOptions.PopulateAppOptionMetadata(sAppOptionName, sRetrievedMetadata);
	// Should return S_FALSE when metadata has not been set
	EXPECT_EQ(sr, S_FALSE);
}

TEST_F(AppOptions, ClearAppOptionMetadata)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Metadata::ToClear");
	static const vlr::tstring sMetadataValue = _T("ClearableMetadata");

	// Set metadata
	sr = m_oAppOptions.SetAppOptionMetadata(sAppOptionName, sMetadataValue);
	EXPECT_EQ(sr, S_OK);

	// Verify it's set
	vlr::tstring sRetrievedMetadata;
	sr = m_oAppOptions.PopulateAppOptionMetadata(sAppOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedMetadata, sMetadataValue);

	// Clear the metadata
	sr = m_oAppOptions.ClearAppOptionMetadata(sAppOptionName);
	EXPECT_EQ(sr, S_OK);

	// Verify it's cleared
	sr = m_oAppOptions.PopulateAppOptionMetadata(sAppOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_FALSE);
}

TEST_F(AppOptions, ClearAppOptionMetadata_NotSet)
{
	SResult sr;

	static const vlr::tstring sAppOptionName = _T("Options::Metadata::NeverSet");

	sr = m_oAppOptions.ClearAppOptionMetadata(sAppOptionName);
	// Note: Clearing metadata that was never set should still succeed (idempotent)
	EXPECT_TRUE(sr.isSuccess());
}

TEST_F(AppOptions, AppOptionMetadata_MultipleOptions)
{
	SResult sr;

	static const vlr::tstring sOptionNameA = _T("Options::Metadata::OptionA");
	static const vlr::tstring sOptionNameB = _T("Options::Metadata::OptionB");
	static const vlr::tstring sMetadataA = _T("MetadataA");
	static const vlr::tstring sMetadataB = _T("MetadataB");

	// Set metadata for different options
	sr = m_oAppOptions.SetAppOptionMetadata(sOptionNameA, sMetadataA);
	EXPECT_EQ(sr, S_OK);

	sr = m_oAppOptions.SetAppOptionMetadata(sOptionNameB, sMetadataB);
	EXPECT_EQ(sr, S_OK);

	// Verify each retrieves correct metadata
	vlr::tstring sRetrievedA, sRetrievedB;

	sr = m_oAppOptions.PopulateAppOptionMetadata(sOptionNameA, sRetrievedA);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedA, sMetadataA);

	sr = m_oAppOptions.PopulateAppOptionMetadata(sOptionNameB, sRetrievedB);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedB, sMetadataB);
}
