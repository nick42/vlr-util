#pragma once

#include <memory>
#include <optional>
#include <map>
#include <shared_mutex>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"
#include "zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( security )

NAMESPACE_BEGIN( SIDs )

class CSidNameLookupResult;
using SPCSidNameLookupResult = std::shared_ptr<CSidNameLookupResult>;

class CSidInfo
{
public:
	PSID m_pSid = nullptr;
	std::optional<vlr::tstring> m_osStringSid;
	SPCSidNameLookupResult m_spSidNameLookupResult;

public:
	inline decltype(auto) With_PSID( PSID pSid )
	{
		m_pSid = pSid;
		m_osStringSid = {};
		return *this;
	}
	inline decltype(auto) With_StringSid( const vlr::tstring& sStringSid )
	{
		m_osStringSid = sStringSid;
		return *this;
	}
	inline decltype(auto) With_SidNameLookupResult( const SPCSidNameLookupResult& spSidNameLookupResult )
	{
		m_spSidNameLookupResult = spSidNameLookupResult;
		return *this;
	}

public:
	HRESULT PopulateData_StringSid();
	HRESULT PopulateData_SidNameLookupResult();

	HRESULT PopulateStringSid( std::optional<vlr::tstring>& osStringSid ) const;
	vlr::tstring GetStringSid();
	vlr::tstring GetStringSid() const;

	vlr::tstring GetDisplay_Default() const;
	std::optional<vlr::tstring> GetDisplay_LogicalAccountName_Default() const;

public:
	CSidInfo() = default;
	CSidInfo( PSID pSid )
		: m_pSid{ pSid }
	{}
};

class CSidNameLookupResult
{
public:
	std::optional<vlr::tstring> m_osLookupSystemName;
	std::optional<DWORD> m_odwLookupError;

	vlr::tstring m_sStringSid;

	std::optional<WELL_KNOWN_SID_TYPE> m_oeWellKnownSid;

	vlr::tstring m_sAccountName;
	vlr::tstring m_sReferencedDomainName;
	SID_NAME_USE m_eUse = SidTypeUnknown;
};

class CSidNameLookupCache
{
protected:
	using TLookupResultStatus = std::pair<bool, SPCSidNameLookupResult>;
	std::map<vlr::tstring, TLookupResultStatus> m_oStringSidToLookupResultMap;
	mutable std::shared_mutex m_oAccessSync_StringSidToLookupResultMap;

	//mutable std::mutex m_oWaitEvent_LookuResultChanged;

public:
	HRESULT GetLookupResult(
		const vlr::tstring& sStringSid,
		SPCSidNameLookupResult& spSidNameLookupResult_Result );

	//struct LookupProcessingToken
	//{
	//	CSidNameLookupCache& oSidNameLookupCache;
	//	vlr::tstring m_sStringSid;
	//	bool m_bAcquiredProccessingToken = false;
	//	std::unique_lock<std::mutex> m_oWaitEventLock;

	//	LookupProcessingToken(
	//		CSidNameLookupCache& oSidNameLookupCache,
	//		const vlr::tstring& sStringSid );
	//};

	//HRESULT AcquireLookupProcessingToken(
	//	const vlr::tstring& sStringSid,
	//	std::shared_ptr<LookupProcessingToken>& spLookupProcessingToken_Result );

	HRESULT SetLookupResult(
		const vlr::tstring& sStringSid,
		const SPCSidNameLookupResult& spSidNameLookupResult );

	HRESULT PopulateCache_WellKnownSids();

public:
	static CSidNameLookupCache& GetSharedInstance();

};

HRESULT DoConvertSidToStringSid(
	PSID pSid,
	vlr::tstring& sStringSid );

HRESULT DoLookupAccountSid(
	LPCTSTR pcszLookupSystemName,
	const CSidInfo& oSidInfo,
	SPCSidNameLookupResult& spSidNameLookupResult_Result );

NAMESPACE_END //( SIDs )

NAMESPACE_END //( security )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
