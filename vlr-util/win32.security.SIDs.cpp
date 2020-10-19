#include "pch.h"
#include <vlr/win32.security.SIDs.h>

#include <sddl.h>

#include "vlr/UtilMacros.Assertions.h"
#include "vlr/AutoFreeResource.h"
#include "vlr/formatpf.h"
#include "vlr/win32.enum.WELL_KNOWN_SID_TYPE.h"
#include "vlr/win32.enum.SID_NAME_USE.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( security )

NAMESPACE_BEGIN( SIDs )

HRESULT CSidInfo::PopulateData_StringSid()
{
	return PopulateStringSid( m_osStringSid );
}

HRESULT CSidInfo::PopulateData_SidNameLookupResult()
{
	return DoLookupAccountSid( nullptr, *this, m_spSidNameLookupResult );
}

HRESULT CSidInfo::PopulateStringSid( std::optional<vlr::tstring>& osStringSid ) const
{
	HRESULT hr;

	vlr::tstring sStringSid;
	hr = DoConvertSidToStringSid( m_pSid, sStringSid );
	ON_HR_NON_S_OK__RETURN_HRESULT( hr );

	osStringSid = sStringSid;

	return S_OK;
}

vlr::tstring CSidInfo::GetStringSid()
{
	if (m_osStringSid.has_value())
	{
		return m_osStringSid.value();
	}
	if (m_pSid == nullptr)
	{
		return _T( "[null]" );
	}

	PopulateStringSid( m_osStringSid );

	return m_osStringSid.value_or( _T( "" ) );
}

vlr::tstring CSidInfo::GetStringSid() const
{
	if (m_osStringSid.has_value())
	{
		return m_osStringSid.value();
	}
	if (m_pSid == nullptr)
	{
		return _T( "[null]" );
	}

	std::optional<vlr::tstring> osStringSid;
	PopulateStringSid( osStringSid );

	return osStringSid.value_or( _T( "" ) );
}

vlr::tstring CSidInfo::GetDisplay_Default() const
{
	vlr::tstring sAccountNamePostfix;
	auto osLogicalAccountName = GetDisplay_LogicalAccountName_Default();
	if (osLogicalAccountName.has_value())
	{
		sAccountNamePostfix = vlr::formatpf( _T( " (%s)" ), osLogicalAccountName.value() );
	}

	return vlr::formatpf( _T( "%s%s" ), GetStringSid(), sAccountNamePostfix );
}

std::optional<vlr::tstring> CSidInfo::GetDisplay_LogicalAccountName_Default() const
{
	if (!m_spSidNameLookupResult)
	{
		return {};
	}

	if (m_spSidNameLookupResult->m_oeWellKnownSid.has_value())
	{
		return vlr::formatpf( _T( "well-known: %s" ),
			enums::CFormatEnum<WELL_KNOWN_SID_TYPE>::FormatValue(m_spSidNameLookupResult->m_oeWellKnownSid.value()) );
	}

	// TODO: More here...

	return vlr::formatpf( _T("[%s] %s\\%s"),
		enums::CFormatEnum<SID_NAME_USE>::FormatValue(m_spSidNameLookupResult->m_eUse),
		m_spSidNameLookupResult->m_sReferencedDomainName,
		m_spSidNameLookupResult->m_sAccountName );
}

HRESULT CSidNameLookupCache::GetLookupResult(
	const vlr::tstring& sStringSid,
	SPCSidNameLookupResult& spSidNameLookupResult_Result )
{
	const auto oLockForRead = std::shared_lock{ m_oAccessSync_StringSidToLookupResultMap };

	auto iterMapIndex = m_oStringSidToLookupResultMap.find( sStringSid );
	if (iterMapIndex == m_oStringSidToLookupResultMap.end())
	{
		return S_FALSE;
	}
	const auto& oLookupResultStatus = iterMapIndex->second;
	if (oLookupResultStatus.first)
	{
		// Lookup in progress for this SID
		return S_FALSE;
	}

	spSidNameLookupResult_Result = oLookupResultStatus.second;

	return S_OK;
}

HRESULT CSidNameLookupCache::SetLookupResult(
	const vlr::tstring& sStringSid,
	const SPCSidNameLookupResult& spSidNameLookupResult )
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( spSidNameLookupResult );

	const auto oLock = std::lock_guard{ m_oAccessSync_StringSidToLookupResultMap };

	m_oStringSidToLookupResultMap[sStringSid] = std::make_pair( false, spSidNameLookupResult );

	return S_OK;
}

HRESULT CSidNameLookupCache::PopulateCache_WellKnownSids()
{
	const auto oLock = std::lock_guard{ m_oAccessSync_StringSidToLookupResultMap };

	HRESULT hr;
	BOOL bSuccess;

	for (UINT nEnumValue = WinNullSid; nEnumValue <= WinBuiltinDeviceOwnersSid; ++nEnumValue)
	{
		auto eWellKnownSid = static_cast<WELL_KNOWN_SID_TYPE>(nEnumValue);

		bool bSidCreateValid = true;
		std::vector<BYTE> oSidDataArray;
		DWORD dwBufferLength = 1024;
		oSidDataArray.resize( 1024 );
		do
		{
			bSuccess = ::CreateWellKnownSid(
				eWellKnownSid,
				NULL,
				oSidDataArray.data(),
				&dwBufferLength );
			if (bSuccess)
			{
				break;
			}
			auto dwLastError = ::GetLastError();
			if (dwLastError == ERROR_INVALID_PARAMETER)
			{
				bSidCreateValid = false;
				break;
			}
			if (dwLastError == ERROR_INSUFFICIENT_BUFFER)
			{
				ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( dwBufferLength, > , oSidDataArray.size() );
				oSidDataArray.resize( dwBufferLength );
				continue;
			}

			HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( HRESULT_FROM_WIN32( dwLastError ) );
		} while (true);
		if (!bSidCreateValid)
		{
			// This well-known Sid is not valid in current context
			continue;
		}

		vlr::tstring sStringSid;
		hr = DoConvertSidToStringSid( oSidDataArray.data(), sStringSid );
		ASSERT_HR_SUCCEEDED__OR_RETURN_HRESULT( hr );

		auto spSidNameLookupResult = std::make_shared<CSidNameLookupResult>();
		ASSERT_ALLOCATED__OR_RETURN_STANDARD_ERROR( spSidNameLookupResult );
		spSidNameLookupResult->m_sStringSid = sStringSid;
		spSidNameLookupResult->m_oeWellKnownSid = eWellKnownSid;

		m_oStringSidToLookupResultMap[sStringSid] = std::make_pair( false, spSidNameLookupResult );
	}

	return S_OK;
}

CSidNameLookupCache& CSidNameLookupCache::GetSharedInstance()
{
	static auto oInstance = CSidNameLookupCache{};

	static const auto bHaveRun_PopulateCache_WellKnownSids = [&]
	{
		oInstance.PopulateCache_WellKnownSids();
		return true;
	}();

	return oInstance;
}

HRESULT DoConvertSidToStringSid(
	PSID pSid,
	vlr::tstring& sStringSid )
{
	static const auto _tFailureValue = E_FAIL;

	// Convert binary SID to string SID

	LPTSTR pszStringSid = nullptr;
	auto bSuccess = ::ConvertSidToStringSid(
		pSid,
		&pszStringSid );
	ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( bSuccess );
	ASSERT_NONZERO__OR_RETURN_FAILURE_VALUE( pszStringSid );
	auto oOnDestroy_FreeStringSid = MakeAutoCleanup_viaLocalFree( pszStringSid );

	sStringSid = vlr::tstring{ pszStringSid };

	return S_OK;
}

HRESULT DoLookupAccountSid(
	LPCTSTR pcszLookupSystemName,
	const CSidInfo& oSidInfo,
	SPCSidNameLookupResult& spSidNameLookupResult_Result )
{
	HRESULT hr;

	auto& oSidNameLookupCache = CSidNameLookupCache::GetSharedInstance();

	hr = oSidNameLookupCache.GetLookupResult(
		oSidInfo.GetStringSid(),
		spSidNameLookupResult_Result );
	ON_HR_S_OK__RETURN_HRESULT( hr );

	auto spSidNameLookupResult = std::make_shared<CSidNameLookupResult>();
	ASSERT_ALLOCATED__OR_RETURN_STANDARD_ERROR( spSidNameLookupResult );
	auto oOnDestroy_AssignResult = MakeActionOnDestruction( [&] { spSidNameLookupResult_Result = spSidNameLookupResult; } );
	auto oOnDestroy_CacheResult = MakeActionOnDestruction( [&]
		{
			hr = oSidNameLookupCache.SetLookupResult(
				oSidInfo.GetStringSid(),
				spSidNameLookupResult );
			ASSERT_HR_SUCCEEDED__OR_CONTINUE( hr );
		} );

	spSidNameLookupResult->m_sStringSid = oSidInfo.GetStringSid();

	BOOL bSuccess;

	DWORD dwBufferLen_AccountName = 0;
	DWORD dwBufferLen_ReferencedDomainName = 0;

	bSuccess = ::LookupAccountSid(
		pcszLookupSystemName,
		oSidInfo.m_pSid,
		nullptr,
		&dwBufferLen_AccountName,
		nullptr,
		&dwBufferLen_ReferencedDomainName,
		&spSidNameLookupResult->m_eUse );
	ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( bSuccess, == , FALSE );
	auto dwLastError = ::GetLastError();
	if (dwLastError != ERROR_INSUFFICIENT_BUFFER)
	{
		spSidNameLookupResult->m_odwLookupError = dwLastError;
		return S_FALSE;
	}

	spSidNameLookupResult->m_sAccountName.resize( dwBufferLen_AccountName );
	spSidNameLookupResult->m_sReferencedDomainName.resize( dwBufferLen_ReferencedDomainName );
	bSuccess = ::LookupAccountSid(
		pcszLookupSystemName,
		oSidInfo.m_pSid,
		spSidNameLookupResult->m_sAccountName.data(),
		&dwBufferLen_AccountName,
		spSidNameLookupResult->m_sReferencedDomainName.data(),
		&dwBufferLen_ReferencedDomainName,
		&spSidNameLookupResult->m_eUse );
	if (!bSuccess)
	{
		spSidNameLookupResult->m_odwLookupError = ::GetLastError();
		return HRESULT_FROM_NT( spSidNameLookupResult->m_odwLookupError.value() );
	}

	spSidNameLookupResult->m_sAccountName.resize( spSidNameLookupResult->m_sAccountName.size() - 1 );
	spSidNameLookupResult->m_sReferencedDomainName.resize( spSidNameLookupResult->m_sReferencedDomainName.size() - 1 );

	return S_OK;
}

NAMESPACE_END //( SIDs )

NAMESPACE_END //( security )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
