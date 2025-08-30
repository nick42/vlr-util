#pragma once

#include "config.h"

#include <optional>
#if VLR_HASDEP_BOOST_ASIO
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>
#endif // VLR_HASDEP_BOOST_ASIO

#include "util.std_aliases.h"
#include "util.convert.StringConversion.h"

namespace vlr {

namespace ops {

class CNetworkTargetInfo
{
public:
	vlr::tstring m_sTargetName_Logical;
	bool m_bIsLocalSystem = false;
	std::optional<vlr::tstring> m_osTargetName_NetBIOS;
	std::optional<vlr::tstring> m_osTargetName_DNS;
#if VLR_HASDEP_BOOST_ASIO
	std::optional<boost::asio::ip::address_v4> m_ooTargetAddress_IPv4;
	std::optional<boost::asio::ip::address_v6> m_ooTargetAddress_IPv6;
#endif // VLR_HASDEP_BOOST_ASIO

public:
	constexpr auto& withIsLocalSystem(bool bValue = true) noexcept
	{
		m_bIsLocalSystem = bValue;
		return *this;
	}
	inline auto& withTargetName_NetBIOS(const vlr::tstring& sTargetName)
	{
		m_osTargetName_NetBIOS = sTargetName;
		return *this;
	}
	inline auto& withTargetName_DNS(const vlr::tstring& sTargetName)
	{
		m_osTargetName_DNS = sTargetName;
		return *this;
	}
#if VLR_HASDEP_BOOST_ASIO
	inline auto& withTargetAddress_IPv4(const boost::asio::ip::address_v4& oTargetAddress)
	{
		m_ooTargetAddress_IPv4 = oTargetAddress;
		return *this;
	}
	inline auto& withTargetAddress_IPv4(const vlr::tstring& sTargetAddress)
	{
		m_ooTargetAddress_IPv4 = boost::asio::ip::make_address_v4(util::Convert::ToStdStringA(sTargetAddress));
		return *this;
	}
	inline auto& withTargetAddress_IPv6(const boost::asio::ip::address_v6& oTargetAddress)
	{
		m_ooTargetAddress_IPv6 = oTargetAddress;
		return *this;
	}
	inline auto& withTargetAddress_IPv6(const vlr::tstring& sTargetAddress)
	{
		m_ooTargetAddress_IPv6 = boost::asio::ip::make_address_v6(util::Convert::ToStdStringA(sTargetAddress));
		return *this;
	}
#endif // VLR_HASDEP_BOOST_ASIO

public:
	vlr::tzstring_view GetNameForIntent_win32_OpenSCManager() const;

public:
	static auto ForLocalSystem()
	{
		return CNetworkTargetInfo{}.withIsLocalSystem();
	}

};

} // namespace ops

} // namespace vlr
