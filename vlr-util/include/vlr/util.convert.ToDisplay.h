#pragma once

#include <string>
#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "util.ModuleContext.Compilation.h"

#include "formatpf.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( Convert )

auto ToDisplay_ApproxDataSize( LONGLONG nSize )
{
	auto fFormatAsSizeType = []( auto nSize, auto nCountForOneOfSizeType, vlr::tzstring_view svzSizeType )
		-> std::optional<vlr::tstring>
	{
		if (nSize > (nCountForOneOfSizeType * 1024LL))
		{
			return {};
		}

		auto dValue = static_cast<double>(nSize) / static_cast<double>(nCountForOneOfSizeType);
		return vlr::formatpf( _T( "%.02f %s" ), dValue, svzSizeType );
	};

	static constexpr auto nCountForOneOfSizeType_Bytes = LONGLONG{ 1LL };
	static constexpr auto nCountForOneOfSizeType_KB = nCountForOneOfSizeType_Bytes * 1024LL;
	static constexpr auto nCountForOneOfSizeType_MB = nCountForOneOfSizeType_KB * 1024LL;
	static constexpr auto nCountForOneOfSizeType_GB = nCountForOneOfSizeType_MB * 1024LL;

	{
		auto osDisplay = fFormatAsSizeType( nSize, nCountForOneOfSizeType_Bytes, _T( "bytes" ) );
		if (osDisplay.has_value())
		{
			return osDisplay.value();
		}
	}
	{
		auto osDisplay = fFormatAsSizeType( nSize, nCountForOneOfSizeType_KB, _T( "KB" ) );
		if (osDisplay.has_value())
		{
			return osDisplay.value();
		}
	}
	{
		auto osDisplay = fFormatAsSizeType( nSize, nCountForOneOfSizeType_MB, _T( "MB" ) );
		if (osDisplay.has_value())
		{
			return osDisplay.value();
		}
	}
	{
		auto osDisplay = fFormatAsSizeType( nSize, nCountForOneOfSizeType_GB, _T( "GB" ) );
		if (osDisplay.has_value())
		{
			return osDisplay.value();
		}
	}

	auto dValue = static_cast<double>(nSize) / static_cast<double>(nCountForOneOfSizeType_GB);
	return vlr::formatpf( _T( "%.02f %s" ), dValue, _T( "GB( big )" ) );
}

NAMESPACE_END //( Convert )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
