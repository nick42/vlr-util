#include "pch.h"

#include "vlr-util/AppOptionSpecifiedValue.h"
#include "vlr-util/StringCompare.h"

using namespace vlr;

static const auto oCompare_OptionName = StringCompare::CI();

TEST(AppOptionSpecifiedValue, PopulateOptionNameElements_DefaultDelimiters)
{
	SResult sr;

	// Check equivalent option names
	{
		static const std::vector<vlr::tstring> vecOptionNames = {
			_T("Options::Path::Name"),
			_T("Options:Path:Name"),
			_T("Options..Path..Name"),
			// Note: Mixed delimiters should also work
			_T("Options.Path.Name"),
			_T("Options::Path.Name"),
			_T("Options.Path::Name"),
			// Note: Mixed case should also work
			_T("Options::path::name"),
			_T("OpTioNs::paTH.Name"),
		};

		for (const auto& sOptionName : vecOptionNames)
		{
			std::vector<vlr::tstring_view> vecOptionNameElements;
			sr = CAppOptionSpecifiedValue::PopulateOptionNameElements_DefaultDelimiters(sOptionName, vecOptionNameElements);
			EXPECT_EQ(sr, S_OK);
			ASSERT_EQ(vecOptionNameElements.size(), 3);
			EXPECT_TRUE(oCompare_OptionName.AreEqual(vecOptionNameElements[0], _T("Options")));
			EXPECT_TRUE(oCompare_OptionName.AreEqual(vecOptionNameElements[1], _T("Path")));
			EXPECT_TRUE(oCompare_OptionName.AreEqual(vecOptionNameElements[2], _T("Name")));
		}
	}
}

TEST(AppOptionSpecifiedValue, CheckForOptionMatch)
{
	auto oValue = CAppOptionSpecifiedValue{}
		.withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue("test")
		;

	EXPECT_EQ(oValue.GetNativeOptionName(), _T("Options::Path::Name"));
	EXPECT_EQ(oValue.GetAppOptionSourceInfo().m_eAppOptionSource, vlr::AppOptionSource::ExplicitViaCode);
	const auto vNativeValue = oValue.GetNativeOptionValue();
	ASSERT_TRUE(std::holds_alternative<std::string>(vNativeValue));
	EXPECT_STREQ(std::get<std::string>(vNativeValue).c_str(), "test");

	{
		const vlr::tstring sNormalizedName = _T("Options::Path::Name");

		auto sr = oValue.CheckForOptionMatch(sNormalizedName);
		EXPECT_EQ(sr, S_OK);
		EXPECT_TRUE(oCompare_OptionName.AreEqual(oValue.GetNormalizedOptionName(), sNormalizedName));
	}

	{
		const vlr::tstring sNormalizedName = _T("Options.Path.Name");

		auto sr = oValue.CheckForOptionMatch(sNormalizedName);
		EXPECT_EQ(sr, S_OK);
		EXPECT_TRUE(oCompare_OptionName.AreEqual(oValue.GetNormalizedOptionName(), sNormalizedName));
	}
}

TEST(AppOptionSpecifiedValue, ExtractOptionValueTo_Int_AsStringA)
{
	auto oValue = CAppOptionSpecifiedValue{}
		.withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue("42")
		;

	{
		std::string tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, "42");
	}

	{
		std::wstring tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, L"42");
	}

	{
		int32_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}

	{
		uint32_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}

	{
		uint64_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}

	{
		double tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}
}

TEST(AppOptionSpecifiedValue, ExtractOptionValueTo_Int)
{
	auto oValue = CAppOptionSpecifiedValue{}
		.withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue(42)
		;

	{
		std::string tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, "42");
	}

	{
		std::wstring tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, L"42");
	}

	{
		int32_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}

	{
		uint32_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}

	{
		uint64_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}

	{
		double tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, 42);
	}
}

TEST(AppOptionSpecifiedValue, ExtractOptionValueTo_int_invalid)
{
	auto oValue = CAppOptionSpecifiedValue{}
		.withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue("SomethingElse")
		;

	{
		int32_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_FALSE);
	}

	{
		uint32_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_FALSE);
	}

	{
		uint64_t tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_FALSE);
	}

	{
		double tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_FALSE);
	}
}

TEST(AppOptionSpecifiedValue, ExtractOptionValueTo_bool)
{
	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("True")
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, true);
	}

	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(L"True")
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, true);
	}

	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("False")
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, false);
	}

	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(L"False")
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, false);
	}

	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(1)
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, true);
	}

	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue(0)
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(tValue, false);
	}

	{
		auto oValue = CAppOptionSpecifiedValue{}
			.withName(_T("Options::Path::Name"))
			.withSource(vlr::AppOptionSource::ExplicitViaCode)
			.withValue("SomethingElse")
			;

		bool tValue{};
		auto sr = oValue.ExtractOptionValueTo<decltype(tValue)>(tValue);
		EXPECT_EQ(sr, S_FALSE);
	}
}

TEST(AppOptionSpecifiedValue, CacheOptionValueAs)
{
	auto oValue = CAppOptionSpecifiedValue{}
		.withName(_T("Options::Path::Name"))
		.withSource(vlr::AppOptionSource::ExplicitViaCode)
		.withValue(42)
		;

	{
		auto sr = oValue.CacheOptionValueAs<std::string>();
		EXPECT_EQ(sr, S_OK);
		EXPECT_NE(std::get_if<std::string>(&oValue.GetCachedOptionValue()), nullptr);
	}

	{
		std::string sValue;
		auto sr = oValue.PopulateFromCachedValue(sValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(sValue, "42");
	}

	EXPECT_NO_THROW(
		auto sValue = oValue.GetCachedValueInline_OrThrow<std::string>();
	);
	EXPECT_ANY_THROW(
		auto sValue = oValue.GetCachedValueInline_OrThrow<std::wstring>();
	);
	EXPECT_ANY_THROW(
		auto sValue = oValue.GetCachedValueInline_OrThrow<int32_t>();
	);
	EXPECT_ANY_THROW(
		auto sValue = oValue.GetCachedValueInline_OrThrow<uint32_t>();
	);
	EXPECT_ANY_THROW(
		auto sValue = oValue.GetCachedValueInline_OrThrow<uint64_t>();
	);
	EXPECT_ANY_THROW(
		auto sValue = oValue.GetCachedValueInline_OrThrow<double>();
	);
}
