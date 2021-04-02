#pragma once

#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>

#include <vlr/UtilMacros.Namespace.h>
#include <vlr/cpp_namespace.h>

#include <vlr/util.convert.StringConversion.h>

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ops )

class CNetworkTargetInfo
{
public:
	cpp::tstring m_sTargetName_Logical;
	bool m_bIsLocalSystem = false;
	std::optional<cpp::tstring> m_osTargetName_NetBIOS;
	std::optional<cpp::tstring> m_osTargetName_DNS;
	std::optional<boost::asio::ip::address_v4> m_ooTargetAddress_IPv4;
	std::optional<boost::asio::ip::address_v6> m_ooTargetAddress_IPv6;

public:
	inline decltype(auto) With_IsLocalSystem( bool bValue = true )
	{
		m_bIsLocalSystem = bValue;
		return *this;
	}
	inline decltype(auto) With_TargetName_NetBIOS( const cpp::tstring& sTargetName )
	{
		m_osTargetName_NetBIOS = sTargetName;
		return *this;
	}
	inline decltype(auto) With_TargetName_DNS( const cpp::tstring& sTargetName )
	{
		m_osTargetName_DNS = sTargetName;
		return *this;
	}
	inline decltype(auto) With_TargetAddress_IPv4( const boost::asio::ip::address_v4& oTargetAddress )
	{
		m_ooTargetAddress_IPv4 = oTargetAddress;
		return *this;
	}
	inline decltype(auto) With_TargetAddress_IPv4( const cpp::tstring& sTargetAddress )
	{
		m_ooTargetAddress_IPv4 = boost::asio::ip::make_address_v4( util::Convert::ToStdStringA( sTargetAddress ) );
		return *this;
	}
	inline decltype(auto) With_TargetAddress_IPv6( const boost::asio::ip::address_v6& oTargetAddress )
	{
		m_ooTargetAddress_IPv6 = oTargetAddress;
		return *this;
	}
	inline decltype(auto) With_TargetAddress_IPv6( const cpp::tstring& sTargetAddress )
	{
		m_ooTargetAddress_IPv6 = boost::asio::ip::make_address_v6( util::Convert::ToStdStringA( sTargetAddress ) );
		return *this;
	}

public:
	cpp::tzstring_view GetNameForIntent_win32_OpenSCManager() const;

public:
	static auto ForLocalSystem()
	{
		return CNetworkTargetInfo{}.With_IsLocalSystem();
	}

};

NAMESPACE_END //( ops )

NAMESPACE_END //( vlr )
