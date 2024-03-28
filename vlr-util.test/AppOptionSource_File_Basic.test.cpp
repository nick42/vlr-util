#include "pch.h"
#include "vlr-util/AppOptionSource_File_Basic.h"

#include <gtest/gtest.h>

namespace vlr {

class AppOptionSource_File_Basic_Test
	: public testing::Test
{
protected:
	CAppOptions m_oAppOptions;
	CAppOptionSource_File_Basic m_oAppOptionSource_File_Basic;

	void SetUp() override
	{
		m_oAppOptionSource_File_Basic.withAppOptionsOverride(&m_oAppOptions);
	}
};

TEST_F(AppOptionSource_File_Basic_Test, ReadAllValuesFromFile)
{
	SResult sr;

	sr = m_oAppOptionSource_File_Basic.ReadAllValuesFromFile(_T("AppOptionSource_basic.txt"));
	ASSERT_TRUE(sr.isSuccess());

	// Obviously, the checks here are tied to the contents of the test file

	auto fValidateOptionNameValue = [&](const auto svzName, const auto svzValue)
	{
		SPCAppOptionSpecifiedValue spSpecifiedValue;
		sr = m_oAppOptions.FindSpecifiedValueByName(svzName, spSpecifiedValue);
		EXPECT_EQ(sr, S_OK);
		ASSERT_NE(spSpecifiedValue, nullptr);
		// For now, this is what the value is added from
		EXPECT_EQ(spSpecifiedValue->GetAppOptionSourceInfo().m_eAppOptionSource, AppOptionSource::BinaryRelatedConfigFile);

		vlr::tstring sValue;
		sr = spSpecifiedValue->ExtractOptionValueTo<vlr::tstring>(sValue);
		EXPECT_EQ(sr, S_OK);
		EXPECT_EQ(sValue, svzValue);
	};

	fValidateOptionNameValue(_T("Name1"), _T("Value1"));
	fValidateOptionNameValue(_T("Name2"), _T("Value2"));
	fValidateOptionNameValue(_T("Name3"), _T(""));
	fValidateOptionNameValue(_T("Name4"), _T("Value with ## inside it"));
	fValidateOptionNameValue(_T("Name5"), _T("Some value"));
	fValidateOptionNameValue(_T("Qualified::Name"), _T("value"));
}

} // namespace vlr
