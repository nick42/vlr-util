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
VLR_DEFINE_APP_OPTION(ValueSetViaCode, uint32_t, 0);

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

TEST(AppOptionAccess, SetSpecifiedValue)
{
	SResult sr;

	{
		auto nValue = Options::AppOptionAccess::ValueSetViaCode{}.GetValueOrDefault();
		EXPECT_EQ(nValue, 0);
	}

	{
		sr = Options::AppOptionAccess::ValueSetViaCode{}.SetSpecifiedValue(42);
		EXPECT_TRUE(sr.isSuccess());
	}

	{
		auto nValue = Options::AppOptionAccess::ValueSetViaCode{}.GetValueOrDefault();
		EXPECT_EQ(nValue, 42);
	}
}

namespace Options::AppOptionAccess {

VLR_DEFINE_APP_OPTION(ReturnOnlyDefaultValue, uint32_t, 42);

}

TEST(AppOptionAccess, OptionWithQualifiers_ReturnOnlyDefaultValue)
{
	SResult sr;

	static const auto sAppOptionName = Options::AppOptionAccess::ReturnOnlyDefaultValue{}.GetOptionName();

	{
		auto spQualifiers = std::make_shared<CAppOptionQualifiers>();
		spQualifiers->withStandardFlagSet(AppOptionQualifiers::StandardFlags::ReturnOnlyDefaultValue);

		CAppOptions::GetSharedInstance().SetAppOptionQualifiers(sAppOptionName, spQualifiers);
	}

	{
		auto spValue = std::make_shared<CAppOptionSpecifiedValue>();
		spValue->withName(sAppOptionName)
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(0)
			;

		sr = CAppOptions::GetSharedInstance().AddSpecifiedValue(spValue);
		EXPECT_EQ(sr, S_OK);
	}

	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = CAppOptions::GetSharedInstance().PopulateSpecifiedValueForOption<uint32_t>(sAppOptionName, spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		ASSERT_NE(spSpecifiedValue, nullptr);

		ASSERT_NE(spSpecifiedValue->GetAppOptionQualifiers(), nullptr);
		EXPECT_EQ(spSpecifiedValue->GetAppOptionQualifiers()->GetFlags_Standard(), AppOptionQualifiers::StandardFlags::ReturnOnlyDefaultValue);

		// The specified value should be 0
		uint32_t nValue{};
		sr = spSpecifiedValue->ExtractOptionValueTo<uint32_t>(nValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(nValue, 0);
	}

	// Option value through accessor should be the default value, not the value we set
	{
		auto nOptionValue = Options::AppOptionAccess::ReturnOnlyDefaultValue{}.GetValueOrDefault();
		EXPECT_EQ(nOptionValue, 42);
	}
}

TEST(AppOptionAccess, EnsureMetadataInStore_SetsAndRetrievesMetadata)
{
	SResult sr;
	static constexpr auto svzOptionName = _T("Options::AppOptionAccess::MetadataTest");
	static constexpr auto sMetadata = _T("Test metadata value");

	// Ensure metadata is stored via the access class
	auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };
	sr = oAppOption.EnsureMetadataInStore(sMetadata);
	EXPECT_EQ(sr, S_OK);

	// Retrieve metadata directly from AppOptions and verify
	vlr::tstring sRetrievedMetadata;
	sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedMetadata, sMetadata);
}

TEST(AppOptionAccess, EnsureMetadataInStore_OverwritesExisting)
{
	SResult sr;
	static constexpr auto svzOptionName = _T("Options::AppOptionAccess::MetadataOverwrite");
	static constexpr auto sMetadata1 = _T("First metadata");
	static constexpr auto sMetadata2 = _T("Second metadata");

	auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };
	sr = oAppOption.EnsureMetadataInStore(sMetadata1);
	EXPECT_EQ(sr, S_OK);

	sr = oAppOption.EnsureMetadataInStore(sMetadata2);
	EXPECT_EQ(sr, S_OK);

	vlr::tstring sRetrievedMetadata;
	sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedMetadata, sMetadata2);
}

TEST(AppOptionAccess, EnsureMetadataInStore_EmptyString)
{
	SResult sr;
	static constexpr auto svzOptionName = _T("Options::AppOptionAccess::MetadataEmpty");

	auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };
	sr = oAppOption.EnsureMetadataInStore(_T(""));
	EXPECT_EQ(sr, S_OK);

	vlr::tstring sRetrievedMetadata;
	sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName, sRetrievedMetadata);
	EXPECT_EQ(sr, S_OK);
	EXPECT_EQ(sRetrievedMetadata, _T(""));
}

TEST(AppOptionAccess, EnsureMetadataInStore_LongString)
{
    SResult sr;
    static constexpr auto svzOptionName = _T("Options::AppOptionAccess::MetadataLong");
    vlr::tstring sLongMetadata(10000, _T('X'));

    auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };
    sr = oAppOption.EnsureMetadataInStore(sLongMetadata);
    EXPECT_EQ(sr, S_OK);

    vlr::tstring sRetrievedMetadata;
    sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName, sRetrievedMetadata);
    EXPECT_EQ(sr, S_OK);
    EXPECT_EQ(sRetrievedMetadata, sLongMetadata);
}

TEST(AppOptionAccess, EnsureMetadataInStore_NonAscii)
{
    SResult sr;
    static constexpr auto svzOptionName = _T("Options::AppOptionAccess::MetadataUnicode");
    vlr::tstring sUnicodeMetadata = _T("测试元数据"); // "test metadata" in Chinese

    auto oAppOption = CAppOptionAccess<uint32_t>{ svzOptionName, 0 };
    sr = oAppOption.EnsureMetadataInStore(sUnicodeMetadata);
    EXPECT_EQ(sr, S_OK);

    vlr::tstring sRetrievedMetadata;
    sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName, sRetrievedMetadata);
    EXPECT_EQ(sr, S_OK);
    EXPECT_EQ(sRetrievedMetadata, sUnicodeMetadata);
}

TEST(AppOptionAccess, EnsureMetadataInStore_MultipleOptions)
{
    SResult sr;
    static constexpr auto svzOptionName1 = _T("Options::AppOptionAccess::MetadataOpt1");
    static constexpr auto svzOptionName2 = _T("Options::AppOptionAccess::MetadataOpt2");
    static constexpr auto sMetadata1 = _T("Meta1");
    static constexpr auto sMetadata2 = _T("Meta2");

    auto oAppOption1 = CAppOptionAccess<uint32_t>{ svzOptionName1, 0 };
    auto oAppOption2 = CAppOptionAccess<uint32_t>{ svzOptionName2, 0 };
    sr = oAppOption1.EnsureMetadataInStore(sMetadata1);
    EXPECT_EQ(sr, S_OK);
    sr = oAppOption2.EnsureMetadataInStore(sMetadata2);
    EXPECT_EQ(sr, S_OK);

    vlr::tstring sRetrievedMetadata1, sRetrievedMetadata2;
    sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName1, sRetrievedMetadata1);
    EXPECT_EQ(sr, S_OK);
    EXPECT_EQ(sRetrievedMetadata1, sMetadata1);

    sr = CAppOptions::GetSharedInstance().PopulateAppOptionMetadata(svzOptionName2, sRetrievedMetadata2);
    EXPECT_EQ(sr, S_OK);
    EXPECT_EQ(sRetrievedMetadata2, sMetadata2);
}
