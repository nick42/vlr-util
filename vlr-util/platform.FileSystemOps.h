#pragma once

#include "config.h"

#include "util.Result.h"
#include "zstring_view.h"

namespace vlr {

namespace platform {

class CFileSystmOps
{
public:
	struct Options_DeleteFile
	{
		bool m_bAllowRelativeFilePath = false;

		VLR_DEFINE_DEFAULT_CONSTRUCTOR_CONSTEXPR_NOEXCEPT(Options_DeleteFile);
	};
	SResult DeleteFile(
		vlr::tzstring_view svzFilePath,
		const Options_DeleteFile& options = {});

	SResult CheckFileExists(vlr::tzstring_view svzFilePath);
	SResult CheckDirectoryExists(vlr::tzstring_view svzFilePath);

	SResult PopulateSystemPath_TempDir(vlr::tstring& sPath);

};

} // namespace platform

} // namespace vlr
