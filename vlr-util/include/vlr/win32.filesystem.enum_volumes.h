#pragma once

#include <optional>

#include <boost/iterator/iterator_facade.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/ActionOnDestruction.h"
#include "vlr/util.std_aliases.h"
#include "vlr/UtilMacros.Assertions.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( win32 )

NAMESPACE_BEGIN( filesystem )

class iterator_volumes
	: public boost::iterator_facade<iterator_volumes, vlr::tstring, boost::forward_traversal_tag, const vlr::tstring&>
{
	friend boost::iterator_core_access;

protected:
	struct RefCountedDataBlock
	{
		std::optional<HANDLE> m_ohFindVolume;
	};
	std::shared_ptr<RefCountedDataBlock> m_spRefCountedDataBlock;
	std::optional<vlr::tstring> m_osCurrentResult;
	std::optional<DWORD> m_odwLastError;

protected:
	HRESULT OnIterationBegin();
	HRESULT OnAdaptorMethod_increment();
	static HRESULT OnDestroy_CloseFindVolume( RefCountedDataBlock* pRefCountedDataBlock );

public:
	inline decltype(auto) With_BeginIteration()
	{
		OnIterationBegin();
		return *this;
	}
	inline decltype(auto) GetLastError() const
	{
		return m_odwLastError;
	}

public:
	auto& dereference() const
	{
		if (!m_osCurrentResult.has_value())
		{
			throw std::exception{ "Invalid iterator state" };
		}
		return m_osCurrentResult.value();
	}
	void increment()
	{
		if (!m_spRefCountedDataBlock || !m_spRefCountedDataBlock->m_ohFindVolume.has_value())
		{
			throw std::exception{ "Invalid iterator state" };
		}
		OnAdaptorMethod_increment();
	}
	auto equal( const iterator_volumes& iterOther ) const
	{
		bool bInvalidIter_this = (!m_spRefCountedDataBlock || !m_spRefCountedDataBlock->m_ohFindVolume.has_value());
		bool bInvalidIter_other = (!iterOther.m_spRefCountedDataBlock || !iterOther.m_spRefCountedDataBlock->m_ohFindVolume.has_value());

		// If either is invalid, then they are equal IFF both are invalid
		if (bInvalidIter_this || bInvalidIter_other)
		{
			return bInvalidIter_this && bInvalidIter_other;
		}

		// Both valid; any appliable checks for validity
		return true
			&& (m_spRefCountedDataBlock->m_ohFindVolume.value() == iterOther.m_spRefCountedDataBlock->m_ohFindVolume.value())
			;
	}

public:
	constexpr iterator_volumes() = default;
	~iterator_volumes() = default;
};

HRESULT iterator_volumes::OnIterationBegin()
{
	vlr::tstring sValue;
	sValue.resize( MAX_PATH );
	auto hFindVolume = ::FindFirstVolume(
		sValue.data(),
		MAX_PATH );
	if (hFindVolume == INVALID_HANDLE_VALUE)
	{
		m_odwLastError = ::GetLastError();
		return E_UNEXPECTED;
	}

	m_spRefCountedDataBlock = std::shared_ptr<RefCountedDataBlock>{ new RefCountedDataBlock, &iterator_volumes::OnDestroy_CloseFindVolume };
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_spRefCountedDataBlock );

	m_spRefCountedDataBlock->m_ohFindVolume = hFindVolume;
	m_osCurrentResult = sValue;

	return S_OK;
}

HRESULT iterator_volumes::OnAdaptorMethod_increment()
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_spRefCountedDataBlock );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_spRefCountedDataBlock->m_ohFindVolume.has_value() );

	m_osCurrentResult = {};

	BOOL bSuccess;

	vlr::tstring sValue;
	sValue.resize( MAX_PATH );
	bSuccess = ::FindNextVolume(
		m_spRefCountedDataBlock->m_ohFindVolume.value(),
		sValue.data(),
		MAX_PATH );
	if (bSuccess)
	{
		m_osCurrentResult = sValue;
		return S_OK;
	}

	m_odwLastError = ::GetLastError();
	if (m_odwLastError.value() != ERROR_NO_MORE_FILES)
	{
		return E_FAIL;
	}

	// Done with the iteration; clear the handle holder (will close here if last reference)
	m_spRefCountedDataBlock = {};

	return S_OK;
}

HRESULT iterator_volumes::OnDestroy_CloseFindVolume( RefCountedDataBlock* pRefCountedDataBlock )
{
	if (!pRefCountedDataBlock)
	{
		return S_FALSE;
	}
	if (!pRefCountedDataBlock->m_ohFindVolume.has_value())
	{
		return S_FALSE;
	}

	auto oOnDestroy_ClearHandleVar = MakeActionOnDestruction( [&] { pRefCountedDataBlock->m_ohFindVolume = {}; } );

	BOOL bSuccess;

	bSuccess = ::FindVolumeClose(
		pRefCountedDataBlock->m_ohFindVolume.value() );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( bSuccess );

	return S_OK;
}

struct enum_volumes
{
	inline auto begin() const
	{
		return iterator_volumes{}.With_BeginIteration();
	}
	inline auto end() const
	{
		return iterator_volumes{};
	}
};

NAMESPACE_END //( filesystem )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
