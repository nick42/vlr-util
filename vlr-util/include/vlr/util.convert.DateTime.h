#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"
#include "UtilMacros.Assertions.h"

#include "util.types.h"
#include "util.convert.StringConversion.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( Convert )

NAMESPACE_BEGIN( detail )

#ifdef WIN32

NAMESPACE_BEGIN( Windows )

inline auto ToDateTime_FILETIME( const FILETIME& oValue )
-> vlr::types::DateTime
{
	static const auto _tFailureValue = vlr::types::DateTime{};

	auto oSystemTime = SYSTEMTIME{};
	auto bSuccess = ::FileTimeToSystemTime( &oValue, &oSystemTime );
	ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( bSuccess );

	return vlr::types::DateTime{ oSystemTime };
}

inline auto ToStdStringA( const vlr::types::DateTime& dtValue )
{
	auto sValue = dtValue.Format();
	return vlr::util::Convert::ToStdStringA( sValue );
}

inline auto ToStdStringW( const vlr::types::DateTime& dtValue )
{
	auto sValue = dtValue.Format();
	return vlr::util::Convert::ToStdStringW( sValue );
}

NAMESPACE_END //( Windows )

#endif

NAMESPACE_END //( detail )

inline auto ToDateTime_FILETIME( const FILETIME& oValue )
-> vlr::types::DateTime
{
	if constexpr (ModuleContext::Compilation::IsPlatform_Windows())
	{
		return detail::Windows::ToDateTime_FILETIME( oValue );
	}
	else
	{
		static_assert("Unsupported platform type");
	}
}

template<>
inline decltype(auto) ToStdStringA( const vlr::types::DateTime& dtValue )
{
	if constexpr (ModuleContext::Compilation::IsPlatform_Windows())
	{
		return detail::Windows::ToStdStringA( dtValue );
	}
	else
	{
		static_assert("Unsupported platform type");
	}
}

template<>
inline decltype(auto) ToStdStringW( const vlr::types::DateTime& dtValue )
{
	if constexpr (ModuleContext::Compilation::IsPlatform_Windows())
	{
		return detail::Windows::ToStdStringW( dtValue );
	}
	else
	{
		static_assert("Unsupported platform type");
	}
}

NAMESPACE_END //( Convert )

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
