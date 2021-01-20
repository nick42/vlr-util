#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "cpp_namespace.h"

#include "UtilMacros.Assertions.h"
#include "zstring_view.h"
#include "util.convert.StringConversion.h"

#include "win32.registry.RegValue.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( registry )

constexpr auto IsBaseKey( HKEY hKey )
{
	// Note: Must use C-style casts here, because C++ is still lacking in some areas...

	switch ((ULONG_PTR)hKey)
	{
	case (ULONG_PTR)HKEY_CLASSES_ROOT:
	case (ULONG_PTR)HKEY_CURRENT_USER:
	case (ULONG_PTR)HKEY_LOCAL_MACHINE:
	case (ULONG_PTR)HKEY_USERS:
	case (ULONG_PTR)HKEY_PERFORMANCE_DATA:
	case (ULONG_PTR)HKEY_PERFORMANCE_TEXT:
	case (ULONG_PTR)HKEY_PERFORMANCE_NLSTEXT:
	case (ULONG_PTR)HKEY_CURRENT_CONFIG:
	case (ULONG_PTR)HKEY_DYN_DATA:
	case (ULONG_PTR)HKEY_CURRENT_USER_LOCAL_SETTINGS:
		return true;

	default:
		return false;
	}
}

struct Options_OpenKey
{
public:
	DWORD m_ulOptions = 0;
	REGSAM m_ulDesiredAccess = KEY_ALL_ACCESS;

public:
	decltype(auto) WithAccess_All()
	{
		m_ulDesiredAccess = KEY_ALL_ACCESS;
		return *this;
	}
	decltype(auto) WithAccess_Read()
	{
		m_ulDesiredAccess = KEY_READ;
		return *this;
	}
	decltype(auto) WithAccess_Write()
	{
		m_ulDesiredAccess = KEY_WRITE;
		return *this;
	}
	decltype(auto) WithAccess_ReadWrite()
	{
		m_ulDesiredAccess = KEY_READ | KEY_WRITE;
		return *this;
	}
};

struct Options_GetValue
{
public:
	DWORD m_dwFlags = 0;
	size_t m_nInitialBufferSize = 1024;

public:
	inline DWORD GetForCall_Flags() const
	{
		auto dwFlags = m_dwFlags;

		// Note: Docs specify that one of the types MUST be specified; '0' does not work in this case.
		auto dwDesiredType = dwFlags & RRF_RT_ANY;
		if (dwDesiredType == 0)
		{
			dwFlags |= RRF_RT_ANY;
		}

		// Note: Cannot specify both this flags together
		static constexpr DWORD dwFlagCombination_32and64Rights = RRF_SUBKEY_WOW6464KEY | RRF_SUBKEY_WOW6432KEY;
		if ((dwFlags & dwFlagCombination_32and64Rights) == dwFlagCombination_32and64Rights)
		{
			vlr::assert::HandleCheckFailure( _T( "RRF_SUBKEY_WOW6464KEY and RRF_SUBKEY_WOW6432KEY both set" ) );
			// Pick one arbitrarily
			dwFlags &= (~dwFlagCombination_32and64Rights);
			dwFlags |= RRF_SUBKEY_WOW6432KEY;
		}

		return dwFlags;
	}

};

struct Result_GetValue
{
	CRegValue m_oValue;
};

class CRegKey
{
protected:
	struct KeyAndCleanupDisposition
	{
		HKEY m_hKey = {};
		bool m_bCloseKey = false;

		KeyAndCleanupDisposition(
			HKEY hKey,
			bool bCloseKey )
			: m_hKey{ hKey }
			, m_bCloseKey{ bCloseKey }
		{}
	};

protected:
	std::optional<HKEY> m_ohBaseKey;
	cpp::shared_ptr<KeyAndCleanupDisposition> m_spKey;

protected:
	static LSTATUS CloseKey( KeyAndCleanupDisposition* pKey )
	{
		if (pKey && pKey->m_bCloseKey)
		{
			return ::RegCloseKey( pKey->m_hKey );
		}
		else
		{
			return 0;
		}
	}
	void SetInternalHKEY( HKEY hKey, bool bCloseKey = true )
	{
		if (vlr::win32::registry::IsBaseKey( hKey ))
		{
			m_ohBaseKey = hKey;
			m_spKey = {};
		}
		else if (hKey != nullptr)
		{
			m_ohBaseKey = {};
			m_spKey = cpp::shared_ptr<KeyAndCleanupDisposition>{ new KeyAndCleanupDisposition( hKey, bCloseKey ), []( auto* pKey ) { CRegKey::CloseKey( pKey ); } };
		}
		else
		{
			Clear();
		}
	}

public:
	void Clear()
	{
		m_ohBaseKey = {};
		m_spKey = {};
	}
	void Attach( HKEY hKey )
	{
		SetInternalHKEY( hKey, true );
	}
	std::optional<HKEY> Detatch()
	{
		if (m_ohBaseKey.has_value())
		{
			return m_ohBaseKey.value();
		}
		if (m_spKey)
		{
			auto hKey = m_spKey->m_hKey;
			m_spKey->m_bCloseKey = false;
			m_spKey = {};
			return hKey;
		}
		return {};
	}

public:
	[[nodiscard]]
	inline std::optional<HKEY> GetHKEY() const
	{
		if (m_ohBaseKey.has_value())
		{
			return m_ohBaseKey.value();
		}
		if (m_spKey)
		{
			return m_spKey->m_hKey;
		}
		return {};
	}
	[[nodiscard]]
	inline bool IsValid() const
	{
		return GetHKEY().has_value();
	}
	[[nodiscard]]
	inline bool IsBaseKey() const
	{
		return IsValid() && m_ohBaseKey.has_value();
	}

protected:
	using FOpenKeyAW = cpp::function<HRESULT( HKEY hKey, const Options_OpenKey& oOptions, HKEY& hkResult )>;
	HRESULT OpenKeyAW(
		const FOpenKeyAW& fOpenKey,
		const Options_OpenKey& oOptions,
		CRegKey& oRegKey_Result );
public:
	template< typename TKeyName, typename std::enable_if_t<std::is_convertible_v<TKeyName, vlr::zstring_view>>* = nullptr >
	HRESULT OpenKey(
		const TKeyName& sKeyName,
		CRegKey& oRegKey_Result,
		const Options_OpenKey& oOptions = {} )
	{
		auto fOpenKey = [&]( HKEY hKey, const Options_OpenKey& oOptions, HKEY& hkResult )
		{
			auto lResult = ::RegOpenKeyExA(
				hKey,
				vlr::zstring_view{ sKeyName },
				oOptions.m_ulOptions,
				oOptions.m_ulDesiredAccess,
				&hkResult );
			ASSERT_COMPARE__OR_RETURN_EXPRESSION( lResult, == , 0, HRESULT_FROM_WIN32( lResult ) );
			ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( hkResult );

			return S_OK;
		};

		return OpenKeyAW( fOpenKey, oOptions, oRegKey_Result );
	}
	template< typename TKeyName, typename std::enable_if_t<std::is_convertible_v<TKeyName, vlr::wzstring_view>>* = nullptr >
	HRESULT OpenKey(
		const TKeyName& sKeyName,
		CRegKey& oRegKey_Result,
		const Options_OpenKey& oOptions = {} )
	{
		auto fOpenKey = [&]( HKEY hKey, const Options_OpenKey& oOptions, HKEY& hkResult )
		{
			auto lResult = ::RegOpenKeyExW(
				hKey,
				vlr::wzstring_view{ sKeyName },
				oOptions.m_ulOptions,
				oOptions.m_ulDesiredAccess,
				&hkResult );
			ASSERT_COMPARE__OR_RETURN_EXPRESSION( lResult, == , 0, HRESULT_FROM_WIN32( lResult ) );
			ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( hkResult );

			return S_OK;
		};

		return OpenKeyAW( fOpenKey, oOptions, oRegKey_Result );
	}

protected:
	using FGetValueAW = cpp::function<HRESULT(
		HKEY hKey,
		const Options_GetValue& oOptions,
		DWORD* pdwType,
		VOID* pBuffer,
		DWORD* pdwBufferLength )>;
	HRESULT GetValueAW(
		const FGetValueAW& fGetValue,
		const Options_GetValue& oOptions,
		Result_GetValue& oResult );
public:
	template< typename TValueName, typename std::enable_if_t<std::is_convertible_v<TValueName, vlr::zstring_view>>* = nullptr >
	HRESULT GetValue(
		vlr::zstring_view svzSubkeyName,
		const TValueName& sValueName,
		Result_GetValue& oResult,
		const Options_GetValue& oOptions = {} )
	{
		oResult.m_oValue.m_wsName = util::Convert::ToStdStringW( sValueName );

		auto fGetValue = [&](
			HKEY hKey,
			const Options_GetValue& oOptions,
			DWORD* pdwType,
			VOID* pBuffer,
			DWORD* pdwBufferLength )
		{
			auto lResult = ::RegGetValueA(
				hKey,
				svzSubkeyName,
				vlr::zstring_view{ sValueName },
				oOptions.GetForCall_Flags(),
				pdwType,
				pBuffer,
				pdwBufferLength );
			switch (lResult)
			{
			case ERROR_SUCCESS:
				return S_OK;
			case ERROR_MORE_DATA:
				return HRESULT_FROM_WIN32( ERROR_MORE_DATA );

			default:
				return HRESULT_FROM_WIN32( lResult );
			}
		};

		return GetValueAW( fGetValue, oOptions, oResult );
	}
	template< typename TValueName, typename std::enable_if_t<std::is_convertible_v<TValueName, vlr::zstring_view>>* = nullptr >
	HRESULT GetValue(
		const TValueName& sValueName,
		Result_GetValue& oResult,
		const Options_GetValue& oOptions = {} )
	{
		return GetValue(
			vlr::zstring_view{},
			sValueName,
			oResult,
			oOptions );
	}
	template< typename TValueName, typename std::enable_if_t<std::is_convertible_v<TValueName, vlr::wzstring_view>>* = nullptr >
	HRESULT GetValue(
		vlr::wzstring_view svzSubkeyName,
		const TValueName& sValueName,
		Result_GetValue& oResult,
		const Options_GetValue& oOptions = {} )
	{
		oResult.m_oValue.m_wsName = util::Convert::ToStdStringW( sValueName );

		auto fGetValue = [&](
			HKEY hKey,
			const Options_GetValue& oOptions,
			DWORD* pdwType,
			VOID* pBuffer,
			DWORD* pdwBufferLength )
		{
			auto lResult = ::RegGetValueW(
				hKey,
				svzSubkeyName,
				vlr::wzstring_view{ sValueName },
				oOptions.GetForCall_Flags(),
				pdwType,
				pBuffer,
				pdwBufferLength );
			switch (lResult)
			{
			case ERROR_SUCCESS:
				return S_OK;
			case ERROR_MORE_DATA:
				return HRESULT_FROM_WIN32( ERROR_MORE_DATA );

			default:
				return HRESULT_FROM_WIN32( lResult );
			}
		};

		return GetValueAW( fGetValue, oOptions, oResult );
	}
	template< typename TValueName, typename std::enable_if_t<std::is_convertible_v<TValueName, vlr::wzstring_view>>* = nullptr >
	HRESULT GetValue(
		const TValueName& sValueName,
		Result_GetValue& oResult,
		const Options_GetValue& oOptions = {} )
	{
		return GetValue(
			vlr::wzstring_view{},
			sValueName,
			oResult,
			oOptions );
	}

public:
	constexpr CRegKey() = default;
	CRegKey( HKEY hKey )
	{
		SetInternalHKEY( hKey );
	}
	//CRegKey( const CRegKey& ) = default;
	//CRegKey( CRegKey&& ) = default;
	~CRegKey() = default;

};

HRESULT CRegKey::OpenKeyAW(
	const FOpenKeyAW& fOpenKey,
	const Options_OpenKey& oOptions,
	CRegKey& oRegKey_Result )
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( fOpenKey );

	HRESULT hr;

	auto ohKey = GetHKEY();
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( ohKey.has_value() );

	HKEY hkResult = {};
	hr = fOpenKey(
		ohKey.value(),
		oOptions,
		hkResult );
	ON_HR_NON_S_OK__RETURN_HRESULT( hr );

	oRegKey_Result = CRegKey{ hkResult };

	return S_OK;
}

HRESULT CRegKey::GetValueAW(
	const FGetValueAW& fGetValue,
	const Options_GetValue& oOptions,
	Result_GetValue& oResult )
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( fGetValue );

	HRESULT hr;

	auto ohKey = GetHKEY();
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( ohKey.has_value() );

	auto& oRegValue = oResult.m_oValue;

	void* pBuffer = nullptr;
	DWORD dwBufferLength = 0;

	if (oOptions.m_nInitialBufferSize > 0)
	{
		oRegValue.m_oData.resize( oOptions.m_nInitialBufferSize );
		pBuffer = oRegValue.m_oData.data();
		dwBufferLength = oRegValue.m_oData.size();
	}

	do
	{
		hr = fGetValue(
			ohKey.value(),
			oOptions,
			&oRegValue.m_dwType,
			pBuffer,
			&dwBufferLength );
		if (hr == S_OK)
		{
			ASSERT_COMPARE__OR_RETURN_EUNEXPECTED( dwBufferLength, <= , oRegValue.m_oData.size() );
			oRegValue.m_oData.resize( dwBufferLength );
			return S_OK;
		}
		if (hr == HRESULT_FROM_WIN32( ERROR_MORE_DATA ))
		{
			if (dwBufferLength > oRegValue.m_oData.size())
			{
				oRegValue.m_oData.resize( dwBufferLength );
				continue;
			}
			// Other case: dynamic data, where we do not know the size
			// No handling for this case; need to specify larger initial buffer in options.
			return hr;
		}

		// No handling for any other case
		// Note: errors are fine; just propagate out
		return hr;
	} while (true);

	// Should not reach here
	HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( E_UNEXPECTED );
}

NAMESPACE_END //( registry )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
