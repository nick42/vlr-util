#include "pch.h"

#include "vlr-util/win32.registry.RegKey.h"
#include "vlr-util/win32.registry.iterator_RegEnumValue.h"
#include "vlr-util/win32.registry.enum_RegValues.h"
#include "vlr-util/win32.registry.enum_RegKeys.h"

TEST( win32_registry_RegKey, general )
{
	HRESULT hr;

	auto oRegKey_HKLM = vlr::win32::registry::CRegKey{ HKEY_LOCAL_MACHINE };
	auto oRegKey_HKCU = vlr::win32::registry::CRegKey{ HKEY_CURRENT_USER };

	auto oOptions_OpenKey_Read = vlr::win32::registry::Options_OpenKey{}.WithAccess_Read();

	{
		vlr::win32::registry::CRegKey oRegKey_HTLM_Software;
		hr = oRegKey_HKLM.OpenKey( "Software", oRegKey_HTLM_Software, oOptions_OpenKey_Read );
		ASSERT_EQ( hr, S_OK );
		ASSERT_EQ( oRegKey_HTLM_Software.IsValid(), true );
	}
	{
		vlr::win32::registry::CRegKey oRegKey_HTLM_Software;
		hr = oRegKey_HKLM.OpenKey( L"Software", oRegKey_HTLM_Software, oOptions_OpenKey_Read );
		ASSERT_EQ( hr, S_OK );
		ASSERT_EQ( oRegKey_HTLM_Software.IsValid(), true );
	}

	static constexpr auto svazTestKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer";
	static constexpr auto svwzTestKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer";

	auto fTestValuesUnderTestKey = [&]( vlr::win32::registry::CRegKey& oRegKey_Test )
	{
		vlr::win32::registry::Result_GetValue oRegValue_Default;
		hr = oRegKey_Test.GetValue( vlr::zstring_view{}, oRegValue_Default );
		EXPECT_EQ( hr, HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) );

		vlr::win32::registry::Result_GetValue oRegValue_UserSignedIn;
		hr = oRegKey_Test.GetValue( _T( "UserSignedIn" ), oRegValue_UserSignedIn );
		EXPECT_EQ( hr, S_OK );
		EXPECT_EQ( oRegValue_UserSignedIn.m_oValue.m_dwType, REG_DWORD );
		EXPECT_EQ( oRegValue_UserSignedIn.m_oValue.m_oData.size(), sizeof(DWORD) );
	};

	{
		vlr::win32::registry::CRegKey oRegKey_Test;
		hr = oRegKey_HKCU.OpenKey( svazTestKey, oRegKey_Test, oOptions_OpenKey_Read );
		ASSERT_EQ( hr, S_OK );
		ASSERT_EQ( oRegKey_Test.IsValid(), true );

		fTestValuesUnderTestKey( oRegKey_Test );
	}
	{
		vlr::win32::registry::CRegKey oRegKey_Test;
		hr = oRegKey_HKCU.OpenKey( svwzTestKey, oRegKey_Test, oOptions_OpenKey_Read );
		ASSERT_EQ( hr, S_OK );
		ASSERT_EQ( oRegKey_Test.IsValid(), true );

		fTestValuesUnderTestKey( oRegKey_Test );
	}

	{
		vlr::win32::registry::CRegKey oRegKey_Test;
		hr = oRegKey_HKCU.OpenKey( svwzTestKey, oRegKey_Test, oOptions_OpenKey_Read );
		ASSERT_EQ( hr, S_OK );
		ASSERT_EQ( oRegKey_Test.IsValid(), true );

		auto iterRegValue = vlr::win32::registry::iterator_RegEnumValue{ oRegKey_Test.GetHKEY().value(), 0 };
		auto endRegValue = vlr::win32::registry::iterator_RegEnumValue{ oRegKey_Test.GetHKEY().value() };

		for (; iterRegValue != endRegValue; ++iterRegValue)
		{
			auto& oRegValue = *iterRegValue;
			TRACE( _T( "Value: %s\n" ), oRegValue.m_wsName.c_str() );
		}

		for (const auto& oRegValue : vlr::win32::registry::enum_RegValues{ oRegKey_Test.GetHKEY().value() })
		{
			TRACE( _T( "Value: %s\n" ), oRegValue.m_wsName.c_str() );
		}
	}

	{
		vlr::win32::registry::CRegKey oRegKey_Test;
		hr = oRegKey_HKCU.OpenKey( svwzTestKey, oRegKey_Test, oOptions_OpenKey_Read );
		ASSERT_EQ( hr, S_OK );
		ASSERT_EQ( oRegKey_Test.IsValid(), true );

		auto iterRegKey = vlr::win32::registry::iterator_RegEnumKey{ oRegKey_Test.GetHKEY().value(), 0 };
		auto endRegKey = vlr::win32::registry::iterator_RegEnumKey{ oRegKey_Test.GetHKEY().value() };

		for (; iterRegKey != endRegKey; ++iterRegKey)
		{
			auto& oRegValue = *iterRegKey;
			TRACE( _T( "Key name: %s\n" ), oRegValue.m_wsName.c_str() );
		}

		for (const auto& oRegKey : vlr::win32::registry::enum_RegKeys{ oRegKey_Test.GetHKEY().value() })
		{
			TRACE( _T( "Key name: %s\n" ), oRegKey.m_wsName.c_str() );
		}
	}
}
