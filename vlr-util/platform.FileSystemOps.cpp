#include "pch.h"
#include "platform.FileSystemOps.h"

#include <filesystem>

#include "logging.LogMessage.h"
#include "util.convert.StringConversion.h"

namespace vlr {

namespace platform {

SResult CFileSystmOps::DeleteFile(
	vlr::tzstring_view svzFilePath,
	const Options_DeleteFile& options /*= {}*/)
{
	auto oFilePath = std::filesystem::path{ svzFilePath.toStdString() };
	if (oFilePath.is_relative())
	{
		if (!options.m_bAllowRelativeFilePath)
		{
			logging::LogMessageFmt(VLR_LOG_CONTEXT_WARNING,
				_T("{}: called with relative path, but that was not specified as allowed (path: {})."),
				_T(__FUNCTION__),
				svzFilePath);
			return E_FAIL;
		}
	}
	if (!std::filesystem::is_regular_file(oFilePath))
	{
		logging::LogMessageFmt(VLR_LOG_CONTEXT_WARNING,
			_T("{}: called with non normal file path (path {})."),
			_T(__FUNCTION__),
			svzFilePath);
		return E_FAIL;
	}

	std::error_code oErrorCode{};
	auto bResult = std::filesystem::remove(oFilePath, oErrorCode);
	if (oErrorCode)
	{
		return SResult::ForCallSpecificResult(oErrorCode.value());
	}
	if (!bResult)
	{
		// ::remove returns false, but no error code, if the file didn't exist
		return SResult::Success_WithNuance;
	}

	return SResult::Success;
}

SResult CFileSystmOps::CheckFileExists(vlr::tzstring_view svzFilePath)
{
	auto oFilePath = std::filesystem::path{ svzFilePath.toStdString() };

	std::error_code oErrorCode{};
	auto bResult = std::filesystem::exists(oFilePath, oErrorCode);
	if (oErrorCode)
	{
		return SResult::ForCallSpecificResult(oErrorCode.value());
	}
	if (!bResult)
	{
		return SResult::Success_WithNuance;
	}

	// Note: Checking for existence before type, because checking type will fail if the object doesn't exist

	if (!std::filesystem::is_regular_file(oFilePath))
	{
		auto eFileType = std::filesystem::status(oFilePath).type();
		using TFileTypeNumberBase = std::underlying_type_t<decltype(eFileType)>;
		logging::LogMessageFmt(VLR_LOG_CONTEXT_WARNING,
			_T("{}: called with non normal file path (path: {}; type: {})."),
			_T(__FUNCTION__),
			svzFilePath,
			(TFileTypeNumberBase)eFileType);
		return E_FAIL;
	}

	return SResult::Success;
}

SResult CFileSystmOps::CheckDirectoryExists(vlr::tzstring_view svzFilePath)
{
	auto oFilePath = std::filesystem::path{ svzFilePath.toStdString() };

	std::error_code oErrorCode{};
	auto bResult = std::filesystem::exists(oFilePath, oErrorCode);
	if (oErrorCode)
	{
		return SResult::ForCallSpecificResult(oErrorCode.value());
	}
	if (!bResult)
	{
		return SResult::Success_WithNuance;
	}

	// Note: Checking for existence before type, because checking type will fail if the object doesn't exist

	if (!std::filesystem::is_directory(oFilePath))
	{
		auto eFileType = std::filesystem::status(oFilePath).type();
		using TFileTypeNumberBase = std::underlying_type_t<decltype(eFileType)>;
		logging::LogMessageFmt(VLR_LOG_CONTEXT_WARNING,
			_T("{}: called with non directory path (path: {}; type: {})."),
			_T(__FUNCTION__),
			svzFilePath,
			(TFileTypeNumberBase)eFileType);
		return E_FAIL;
	}

	return SResult::Success;
}

SResult CFileSystmOps::PopulateSystemPath_TempDir(vlr::tstring& sPath)
{
	std::error_code oErrorCode{};
	auto oPath = std::filesystem::temp_directory_path(oErrorCode);
	if (oErrorCode)
	{
		return SResult::ForCallSpecificResult(oErrorCode.value());
	}

	sPath = util::Convert::ToStdString(oPath.c_str());

	return SResult::Success;
}

} // namespace platform

} // namespace vlr
