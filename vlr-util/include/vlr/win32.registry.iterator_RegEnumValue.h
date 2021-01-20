#pragma once

#include <optional>

#include <boost/iterator/iterator_facade.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/ActionOnDestruction.h"
#include "vlr/util.std_aliases.h"
#include "vlr/UtilMacros.Assertions.h"

#include "vlr/win32.registry.RegValue.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( registry )

class enum_RegValues;

class iterator_RegEnumValue
	: public boost::iterator_facade<iterator_RegEnumValue, const CRegValue&, boost::forward_traversal_tag, const CRegValue&>
{
	friend boost::iterator_core_access;
	friend enum_RegValues;

protected:
	HKEY m_hKey = {};
	std::optional<DWORD> m_odwNextIndex;
	cpp::shared_ptr<CRegValue> m_spCurrentRegValue;
	std::optional<DWORD> m_odwLastError;

protected:
	inline bool HaveValidItem() const
	{
		return true
			&& m_odwNextIndex.has_value()
			&& m_spCurrentRegValue
			;
	}
	inline bool HaveValidIndexForIteration() const
	{
		return true
			&& m_odwNextIndex.has_value()
			;
	}
	HRESULT OnAdaptorMethod_increment();

public:
	inline decltype(auto) GetLastError() const
	{
		return m_odwLastError;
	}

public:
	auto dereference() const -> const CRegValue&
	{
		if (!HaveValidItem())
		{
			throw std::exception{ "Invalid iterator state" };
		}
		return *m_spCurrentRegValue;
	}
	void increment()
	{
		if (!HaveValidIndexForIteration())
		{
			throw std::exception{ "Invalid iterator state" };
		}
		OnAdaptorMethod_increment();
	}
	auto equal( const iterator_RegEnumValue& iterOther ) const
	{
		bool bInvalidIter_this = (!HaveValidIndexForIteration());
		bool bInvalidIter_other = (!iterOther.HaveValidIndexForIteration());

		// If either is invalid, then they are equal IFF both are invalid
		if (bInvalidIter_this || bInvalidIter_other)
		{
			return bInvalidIter_this && bInvalidIter_other;
		}

		// Both valid; any appliable checks for validity

		return true
			&& (m_hKey == iterOther.m_hKey)
			&& (m_odwNextIndex.value() == iterOther.m_odwNextIndex.value())
			;
	}

public:
	constexpr iterator_RegEnumValue(
		HKEY hKey )
		: m_hKey{ hKey }
	{}
	iterator_RegEnumValue(
		HKEY hKey,
		DWORD dwIndex )
		: m_hKey{ hKey }
		, m_odwNextIndex{ dwIndex }
	{
		increment();
	}
	~iterator_RegEnumValue() = default;
};

HRESULT iterator_RegEnumValue::OnAdaptorMethod_increment()
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_hKey );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_odwNextIndex.has_value() );

	do
	{
		auto spRegValue = cpp::make_shared<CRegValue>();
		ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( spRegValue );

		spRegValue->m_odwOnEnum_Index = m_odwNextIndex.has_value();

		DWORD dwValueNameLength = 256;
		spRegValue->m_wsName.resize( dwValueNameLength );

		DWORD dwValueLength = 2048;
		spRegValue->m_oData.resize( dwValueLength );

		auto lStatus = ::RegEnumValueW(
			m_hKey,
			m_odwNextIndex.value(),
			spRegValue->m_wsName.data(),
			&dwValueNameLength,
			NULL,
			&spRegValue->m_dwType,
			spRegValue->m_oData.data(),
			&dwValueLength );
		if (lStatus == ERROR_SUCCESS)
		{
			spRegValue->m_wsName.resize( dwValueNameLength );
			spRegValue->m_oData.resize( dwValueLength );
			m_spCurrentRegValue = spRegValue;
			m_odwNextIndex = ++m_odwNextIndex.value();

			return S_OK;
		}
		else if (lStatus == ERROR_NO_MORE_ITEMS)
		{
			m_odwNextIndex = {};
			m_spCurrentRegValue = {};
			m_odwLastError = HRESULT_FROM_WIN32( ERROR_NO_MORE_ITEMS );

			return S_FALSE;
		}
		else if (lStatus == ERROR_MORE_DATA)
		{
			spRegValue->m_wsName.resize( dwValueNameLength );
			spRegValue->m_oData.resize( dwValueLength );

			continue;
		}
		else
		{
			m_odwLastError = HRESULT_FROM_WIN32( lStatus );
			return m_odwLastError.value();
		}
	} while (true);

	HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( E_UNEXPECTED );
}

NAMESPACE_END //( registry )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
