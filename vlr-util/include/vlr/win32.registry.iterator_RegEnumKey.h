#pragma once

#include <optional>

#include <boost/iterator/iterator_facade.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/ActionOnDestruction.h"
#include "vlr/util.std_aliases.h"
#include "vlr/UtilMacros.Assertions.h"

#include "vlr/win32.registry.RegKey.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( registry )

struct RegEnumKeyResult
	: public CRegKey
{
	DWORD m_dwIndex = 0;
	std::wstring m_wsName;
	std::wstring m_wsClass;
	FILETIME m_oLastWriteTime = {};
};

class enum_RegKeys;

class iterator_RegEnumKey
	: public boost::iterator_facade<iterator_RegEnumKey, const RegEnumKeyResult&, boost::forward_traversal_tag, const RegEnumKeyResult&>
{
	friend boost::iterator_core_access;
	friend enum_RegKeys;

protected:
	HKEY m_hParentKey = {};
	std::optional<DWORD> m_odwNextIndex;
	cpp::shared_ptr<RegEnumKeyResult> m_spCurrentResult;
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
	auto equal( const iterator_RegEnumKey& iterOther ) const
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
	constexpr iterator_RegEnumKey(
		HKEY hParentKey )
		: m_hParentKey{ hParentKey }
	{}
	iterator_RegEnumKey(
		HKEY hParentKey,
		DWORD dwIndex )
		: m_hParentKey{ hParentKey }
		, m_odwNextIndex{ dwIndex }
	{
		increment();
	}
	~iterator_RegEnumKey() = default;
};

HRESULT iterator_RegEnumKey::OnAdaptorMethod_increment()
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_hParentKey );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_odwNextIndex.has_value() );

	do
	{
		auto spCurrentResult = cpp::make_shared<RegEnumKeyResult>();
		ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( spCurrentResult );

		spCurrentResult->m_dwIndex = m_odwNextIndex.has_value();

		DWORD dwValueNameLength = 256;
		spCurrentResult->m_wsName.resize( dwValueNameLength );

		DWORD dwClassNameLength = 256;
		spCurrentResult->m_wsClass.resize( dwClassNameLength );

		auto lStatus = ::RegEnumKeyExW(
			m_hParentKey,
			m_odwNextIndex.value(),
			spCurrentResult->m_wsName.data(),
			&dwValueNameLength,
			NULL,
			spCurrentResult->m_wsClass.data(),
			&dwClassNameLength,
			&spCurrentResult->m_oLastWriteTime );
		if (lStatus == ERROR_SUCCESS)
		{
			spCurrentResult->m_wsName.resize( dwValueNameLength );
			spCurrentResult->m_wsClass.resize( dwClassNameLength );
			m_spCurrentResult = spCurrentResult;
			m_odwNextIndex = ++m_odwNextIndex.value();

			return S_OK;
		}
		else if (lStatus == ERROR_NO_MORE_ITEMS)
		{
			m_odwNextIndex = {};
			spCurrentResult = {};
			m_odwLastError = HRESULT_FROM_WIN32( ERROR_NO_MORE_ITEMS );

			return S_FALSE;
		}
		else if (lStatus == ERROR_MORE_DATA)
		{
			spCurrentResult->m_wsName.resize( dwValueNameLength );
			spCurrentResult->m_wsClass.resize( dwClassNameLength );

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
