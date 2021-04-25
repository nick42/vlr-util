#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "cpp_namespace.h"

#include "UtilMacros.Assertions.h"
#include "zstring_view.h"

#include "util.data_adaptor.MultiSZ.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( registry )

class CRegValue
{
public:
	std::wstring m_wsName;
	DWORD m_dwType = 0;
	std::vector<BYTE> m_oData;

	std::optional<DWORD> m_odwOnEnum_Index;

public:
	inline auto GetValue_DWORD() const -> std::optional<DWORD>
	{
		if (m_oData.size() != sizeof( DWORD ))
		{
			return {};
		}
		switch (m_dwType)
		{
		case 0:
		case REG_DWORD:
			break;

		default:
			return {};
		}

		return *reinterpret_cast<const DWORD*>(m_oData.data());
	}
	inline auto GetValue_QWORD() const -> std::optional<ULONGLONG>
	{
		if (m_oData.size() != sizeof( ULONGLONG ))
		{
			return {};
		}
		switch (m_dwType)
		{
		case 0:
		case REG_QWORD:
			break;

		default:
			return {};
		}

		return *reinterpret_cast<const ULONGLONG*>(m_oData.data());
	}
	inline auto GetValue_SZ() const -> std::optional<vlr::wzstring_view>
	{
		static constexpr auto _tFailureValue = std::optional<vlr::wzstring_view>{};

		if (m_oData.size() == 0)
		{
			// Valid string will still have NULL terminator
			return _tFailureValue;
		}
		switch (m_dwType)
		{
		case 0:
		case REG_SZ:
		case REG_EXPAND_SZ:
		case REG_LINK:
			break;

		default:
			return {};
		}

		auto pcwszValue = reinterpret_cast<LPCWSTR>(m_oData.data());
		auto nValueLengthChars = m_oData.size() / sizeof( wchar_t );
		ASSERT_COMPARE__OR_RETURN_FAILURE_VALUE( nValueLengthChars, >= , 1 );
		ASSERT_COMPARE__OR_RETURN_FAILURE_VALUE( pcwszValue[nValueLengthChars - 1], == , L'\0' );

		return vlr::wzstring_view{ pcwszValue, nValueLengthChars, vlr::wzstring_view::StringIsNullTerminated{} };
	}
	inline auto GetValue_MultiSZ() const -> std::optional<std::vector<vlr::wzstring_view>>
	{
		static const auto _tFailureValue = std::optional<std::vector<vlr::wzstring_view>>{};

		if (m_oData.size() == 0)
		{
			// Valid string will still have NULL terminator
			return _tFailureValue;
		}
		switch (m_dwType)
		{
		case 0:
		case REG_MULTI_SZ:
			break;

		default:
			return {};
		}

		auto pcwszValue = reinterpret_cast<LPCWSTR>(m_oData.data());
		auto nValueLengthChars = m_oData.size() / sizeof( wchar_t );
		ASSERT_COMPARE__OR_RETURN_FAILURE_VALUE( nValueLengthChars, >= , 1 );
		ASSERT_COMPARE__OR_RETURN_FAILURE_VALUE( pcwszValue[nValueLengthChars - 1], == , L'\0' );

		HRESULT hr;

		std::vector<vlr::wzstring_view> oValueCollection;
		hr = util::data_adaptor::HelperFor_MultiSZ<wchar_t>{}.ToStructuredData( pcwszValue, oValueCollection );
		ASSERT_HR_SUCCEEDED__OR_RETURN_FAILURE_VALUE( hr );

		return std::move( oValueCollection );
	}

public:
	HRESULT SetValue_DWORD( DWORD dwValue )
	{
		m_dwType = REG_DWORD;
		m_oData = {};
		m_oData.resize( sizeof( DWORD ) );
		memcpy_s( m_oData.data(), m_oData.size(), &dwValue, sizeof( DWORD ) );

		return S_OK;
	}
	HRESULT SetValue_SZ( vlr::wzstring_view svzValue )
	{
		m_dwType = REG_SZ;
		auto nValueLengthBytes = (svzValue.size() + 1) * sizeof( wchar_t );
		m_oData = {};
		m_oData.resize( nValueLengthBytes );
		memcpy_s( m_oData.data(), m_oData.size(), svzValue.data(), nValueLengthBytes );

		return S_OK;
	}
};

NAMESPACE_END //( registry )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
