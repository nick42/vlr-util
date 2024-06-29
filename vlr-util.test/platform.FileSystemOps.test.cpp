#include "pch.h"

#include <filesystem>
#include <fstream>

#include "vlr-util/platform.FileSystemOps.h"
#include "vlr-util/util.convert.StringConversion.h"
#include "vlr-util/UtilMacros.Assertions.h"

using namespace vlr;

// Note: Test functions for file system operations are inherently dependent on the platform...

struct TestFileSystemOps
	: public testing::Test
{
	platform::CFileSystmOps m_oFileSystmOps;

	vlr::tstring m_sPath_TempDir;
	vlr::tstring m_sRandomFileName;
	vlr::tstring m_sPath_ValidTempFile;
	vlr::tstring m_sPath_InvalidTempFile;

	static SResult GenerateRandomFileName(vlr::tstring& sFileName)
	{
		static const std::vector<TCHAR> vecCharacterArray = []()
		{
			std::vector<TCHAR> vecCharacterArray;
			for (int i = 0; i < 26; ++i)
			{
				vecCharacterArray.push_back(_T('A') + i);
			}
			for (int i = 0; i < 10; ++i)
			{
				vecCharacterArray.push_back(_T('0') + i);
			}

			return vecCharacterArray;
		}();

		static constexpr size_t nFileNameLength = 16;

		sFileName.reserve(nFileNameLength);
		for (size_t i = 0; i < nFileNameLength; ++i)
		{
			sFileName.push_back(vecCharacterArray[std::rand() % vecCharacterArray.size()]);
		}

		return SResult::Success;
	}

	static SResult GenerateRandomTempFilePath(vlr::tstring& sFilePath)
	{
		SResult sr;

		vlr::tstring sPath_TempDir;
		sr = platform::CFileSystmOps{}.PopulateSystemPath_TempDir(sPath_TempDir);
		VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
		VLR_ASSERT_NOTBLANK_OR_RETURN_EUNEXPECTED(sPath_TempDir);

		vlr::tstring sRandomFileName;
		sr = GenerateRandomFileName(sRandomFileName);
		EXPECT_EQ(sr, S_OK);

		auto oPath_TempFile = std::filesystem::path{ sPath_TempDir };
		oPath_TempFile.append(sRandomFileName);
		sFilePath = util::Convert::ToStdString(oPath_TempFile.c_str());

		return SResult::Success;
	}

	virtual void SetUp() override
	{
		SResult sr;

		sr = platform::CFileSystmOps{}.PopulateSystemPath_TempDir(m_sPath_TempDir);
		EXPECT_EQ(sr, S_OK);
		EXPECT_STRNE(m_sPath_TempDir.c_str(), _T(""));

		sr = GenerateRandomTempFilePath(m_sPath_ValidTempFile);
		EXPECT_EQ(sr, S_OK);
		EXPECT_STRNE(m_sPath_ValidTempFile.c_str(), _T(""));

		sr = GenerateRandomTempFilePath(m_sPath_InvalidTempFile);
		EXPECT_EQ(sr, S_OK);
		EXPECT_STRNE(m_sPath_InvalidTempFile.c_str(), _T(""));

		// Touch the valid file
		{
			std::ofstream streamTempFile{ m_sPath_ValidTempFile };
		}
	}
	virtual void TearDown() override
	{
		if (m_sPath_ValidTempFile != _T(""))
		{
			std::filesystem::remove(m_sPath_ValidTempFile);
		}
	}
};

TEST_F(TestFileSystemOps, DeleteFile)
{
	SResult sr;

	vlr::tstring sPath_TempFile;
	sr = GenerateRandomTempFilePath(sPath_TempFile);
	EXPECT_EQ(sr, S_OK);
	EXPECT_STRNE(sPath_TempFile.c_str(), _T(""));

	EXPECT_NO_THROW(
		std::ofstream streamTempFile{ m_sPath_ValidTempFile };
	);

	sr = m_oFileSystmOps.CheckFileExists(sPath_TempFile);
	EXPECT_EQ(sr, S_OK);

	sr = m_oFileSystmOps.DeleteFile(sPath_TempFile);
	EXPECT_EQ(sr, S_OK);

	sr = m_oFileSystmOps.CheckFileExists(sPath_TempFile);
	EXPECT_EQ(sr, S_FALSE);
}

TEST_F(TestFileSystemOps, CheckFileExists)
{
	SResult sr;

	sr = m_oFileSystmOps.CheckFileExists(m_sPath_TempDir);
	EXPECT_EQ(sr, S_FALSE);

	sr = m_oFileSystmOps.CheckFileExists(m_sPath_ValidTempFile);
	EXPECT_EQ(sr, S_OK);

	sr = m_oFileSystmOps.CheckFileExists(m_sPath_InvalidTempFile);
	EXPECT_EQ(sr, S_OK);
}

TEST_F(TestFileSystemOps, CheckDirectoryExists)
{
	SResult sr;

	sr = m_oFileSystmOps.CheckFileExists(m_sPath_TempDir);
	EXPECT_EQ(sr, S_OK);

	sr = m_oFileSystmOps.CheckFileExists(m_sPath_ValidTempFile);
	EXPECT_EQ(sr, S_FALSE);

	sr = m_oFileSystmOps.CheckFileExists(m_sPath_InvalidTempFile);
	EXPECT_EQ(sr, S_FALSE);
}
