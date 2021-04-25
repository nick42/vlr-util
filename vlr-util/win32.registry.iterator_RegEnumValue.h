#pragma once

#include <optional>

#include <boost/iterator/iterator_facade.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "ActionOnDestruction.h"
#include "util.std_aliases.h"
#include "UtilMacros.Assertions.h"

#include "win32.registry.RegValue.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( registry )

struct RegEnumValueResult
	: public CRegValue
{
	DWORD m_dwIndex = 0;
};

class enum_RegValues;

class iterator_RegEnumValue
	: public boost::iterator_facade<iterator_RegEnumValue, const RegEnumValueResult&, boost::forward_traversal_tag, const RegEnumValueResult&>
{
	friend boost::iterator_core_access;
	friend enum_RegValues;

protected:
	HKEY m_hParentKey = {};
	std::optional<DWORD> m_odwNextIndex;
	cpp::shared_ptr<RegEnumValueResult> m_spCurrentResult;
	std::optional<DWORD> m_odwLastError;

protected:
	inline bool HaveValidItem() const
	{
		return true
			&& m_odwNextIndex.has_value()
			&& m_spCurrentResult
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
	decltype(auto) dereference() const
	{
		if (!HaveValidItem())
		{
			throw std::exception{ "Invalid iterator state" };
		}
		return *m_spCurrentResult;
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
			&& (m_hParentKey == iterOther.m_hParentKey)
			&& (m_odwNextIndex.value() == iterOther.m_odwNextIndex.value())
			;
	}

public:
	constexpr iterator_RegEnumValue(
		HKEY hParentKey )
		: m_hParentKey{ hParentKey }
	{}
	iterator_RegEnumValue(
		HKEY hParentKey,
		DWORD dwIndex )
		: m_hParentKey{ hParentKey }
		, m_odwNextIndex{ dwIndex }
	{
		increment();
	}
	~iterator_RegEnumValue() = default;
};

HRESULT iterator_RegEnumValue::OnAdaptorMethod_increment()
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_hParentKey );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_odwNextIndex.has_value() );

	do
	{
		auto spCurrentResult = cpp::make_shared<RegEnumValueResult>();
		ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( spCurrentResult );

		spCurrentResult->m_dwIndex = m_odwNextIndex.has_value();

		DWORD dwValueNameLength = 256;
		spCurrentResult->m_wsName.resize( dwValueNameLength );

		DWORD dwValueLength = 2048;
		spCurrentResult->m_oData.resize( dwValueLength );

		auto lStatus = ::RegEnumValueW(
			m_hParentKey,
			m_odwNextIndex.value(),
			spCurrentResult->m_wsName.data(),
			&dwValueNameLength,
			NULL,
			&spCurrentResult->m_dwType,
			spCurrentResult->m_oData.data(),
			&dwValueLength );
		if (lStatus == ERROR_SUCCESS)
		{
			spCurrentResult->m_wsName.resize( dwValueNameLength );
			spCurrentResult->m_oData.resize( dwValueLength );
			m_spCurrentResult = spCurrentResult;
			m_odwNextIndex = ++m_odwNextIndex.value();

			return S_OK;
		}
		else if (lStatus == ERROR_NO_MORE_ITEMS)
		{
			m_odwNextIndex = {};
			m_spCurrentResult = {};
			m_odwLastError = HRESULT_FROM_WIN32( ERROR_NO_MORE_ITEMS );

			return S_FALSE;
		}
		else if (lStatus == ERROR_MORE_DATA)
		{
			spCurrentResult->m_wsName.resize( dwValueNameLength );
			spCurrentResult->m_oData.resize( dwValueLength );

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
