#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "ModuleContext.Compilation.h"
#include "UtilMacros.Assertions.h"

#include "util.types.h"
#include "util.convert.StringConversion.h"

namespace vlr {

namespace util {

namespace Convert {

enum class DateTimeFormat
{
	Unknown,
	ISO8601,
};

struct FormatOptions_DateTime
{
	DateTimeFormat m_eDateTimeFormat = DateTimeFormat::Unknown;

	FormatOptions_DateTime() = default;
	FormatOptions_DateTime( DateTimeFormat eDateTimeFormat )
		: m_eDateTimeFormat{ eDateTimeFormat }
	{}
};

namespace detail {

#ifdef WIN32

namespace Windows {

inline auto ToDateTime_FILETIME( const FILETIME& oValue )
-> vlr::types::DateTime
{
	static const auto _tFailureValue = vlr::types::DateTime{};

	auto oSystemTime = SYSTEMTIME{};
	auto bSuccess = ::FileTimeToSystemTime( &oValue, &oSystemTime );
	VLR_ASSERT_NONZERO_OR_RETURN_FAILURE_VALUE( bSuccess );

	return vlr::types::DateTime{ oSystemTime };
}

inline auto ToString( const vlr::types::DateTime& dtValue, const FormatOptions_DateTime& oFormatOptions )
{
	switch (oFormatOptions.m_eDateTimeFormat)
	{
	default:
	case DateTimeFormat::Unknown:
		return dtValue.Format();
	case DateTimeFormat::ISO8601:
	{
		// Format: YYYY-MM-DD[T]HH:MM:SS[TZ]
		return dtValue.Format( _T( "%Y-%m-%dT%H:%M:%SZ" ) );
	}
	}
}

inline auto ToStdStringA( const vlr::types::DateTime& dtValue, const FormatOptions_DateTime& oFormatOptions )
{
	auto sValue = ToString( dtValue, oFormatOptions );
	return vlr::util::Convert::ToStdStringA( sValue );
}

inline auto ToStdStringW( const vlr::types::DateTime& dtValue, const FormatOptions_DateTime& oFormatOptions )
{
	auto sValue = ToString( dtValue, oFormatOptions );
	return vlr::util::Convert::ToStdStringW( sValue );
}

} // namespace Windows

#endif

} // namespace detail

inline auto ToDateTime_FILETIME(const FILETIME& oValue)
-> vlr::types::DateTime
{
	if constexpr (ModuleContext::Compilation::IsPlatform_Windows())
	{
		return detail::Windows::ToDateTime_FILETIME(oValue);
	}
	else
	{
		static_assert("Unsupported platform type");
	}
}

inline decltype(auto) ToStdStringA(const vlr::types::DateTime& dtValue, const FormatOptions_DateTime& oFormatOptions)
{
	if constexpr (ModuleContext::Compilation::IsPlatform_Windows())
	{
		return detail::Windows::ToStdStringA(dtValue, oFormatOptions);
	}
	else
	{
		static_assert("Unsupported platform type");
	}
}

inline decltype(auto) ToStdStringW(const vlr::types::DateTime& dtValue, const FormatOptions_DateTime& oFormatOptions)
{
	if constexpr (ModuleContext::Compilation::IsPlatform_Windows())
	{
		return detail::Windows::ToStdStringW(dtValue, oFormatOptions);
	}
	else
	{
		static_assert("Unsupported platform type");
	}
}

template<>
inline decltype(auto) ToStdStringA(const vlr::types::DateTime& dtValue)
{
	return ToStdStringA(dtValue, FormatOptions_DateTime{});
}

template<>
inline decltype(auto) ToStdStringW(const vlr::types::DateTime& dtValue)
{
	return ToStdStringW(dtValue, FormatOptions_DateTime{});
}

} // namespace Convert

} // namespace util

} // namespace vlr
