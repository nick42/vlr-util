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

class enum_files;

class iterator_FindNextFile
	: public boost::iterator_facade<iterator_FindNextFile, const WIN32_FIND_DATA*, boost::forward_traversal_tag, const WIN32_FIND_DATA*>
{
	friend boost::iterator_core_access;
	friend enum_files;

protected:
	struct RefCountedDataBlock
	{
		std::optional<HANDLE> m_ohFindHandle;

		RefCountedDataBlock( HANDLE hFindHandle )
			: m_ohFindHandle{ hFindHandle }
		{}
	};
	std::shared_ptr<RefCountedDataBlock> m_spRefCountedDataBlock;
	std::shared_ptr<BYTE[]> m_spResultDataBuffer;
	std::optional<DWORD> m_odwLastError;

protected:
	HRESULT OnAdaptorMethod_increment();
	static HRESULT OnDestroy_FindClose( RefCountedDataBlock* pRefCountedDataBlock );

public:
	inline decltype(auto) GetLastError() const
	{
		return m_odwLastError;
	}

public:
	auto dereference() const
	{
		if (!m_spResultDataBuffer)
		{
			throw std::exception{ "Invalid iterator state" };
		}
		return reinterpret_cast<const WIN32_FIND_DATA*>(m_spResultDataBuffer.get());
	}
	void increment()
	{
		if (!m_spRefCountedDataBlock || !m_spRefCountedDataBlock->m_ohFindHandle.has_value())
		{
			throw std::exception{ "Invalid iterator state" };
		}
		OnAdaptorMethod_increment();
	}
	auto equal( const iterator_FindNextFile& iterOther ) const
	{
		bool bInvalidIter_this = (!m_spRefCountedDataBlock || !m_spRefCountedDataBlock->m_ohFindHandle.has_value());
		bool bInvalidIter_other = (!iterOther.m_spRefCountedDataBlock || !iterOther.m_spRefCountedDataBlock->m_ohFindHandle.has_value());

		// If either is invalid, then they are equal IFF both are invalid
		if (bInvalidIter_this || bInvalidIter_other)
		{
			return bInvalidIter_this && bInvalidIter_other;
		}

		// Both valid; any appliable checks for validity
		return true
			&& (m_spRefCountedDataBlock->m_ohFindHandle.value() == iterOther.m_spRefCountedDataBlock->m_ohFindHandle.value())
			;
	}

public:
	constexpr iterator_FindNextFile() = default;
	~iterator_FindNextFile() = default;
};

//HRESULT iterator_files::OnIterationBegin()
//{
//	vlr::tstring sValue;
//	sValue.resize( MAX_PATH );
//	auto hFindVolume = ::FindFirstVolume(
//		sValue.data(),
//		MAX_PATH );
//	if (hFindVolume == INVALID_HANDLE_VALUE)
//	{
//		m_odwLastError = ::GetLastError();
//		return E_UNEXPECTED;
//	}
//
//	m_spRefCountedDataBlock = std::shared_ptr<RefCountedDataBlock>{ new RefCountedDataBlock, &iterator_files::OnDestroy_FindClose };
//	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_spRefCountedDataBlock );
//
//	m_spRefCountedDataBlock->m_ohFindHandle = hFindVolume;
//	m_osCurrentResult = sValue;
//
//	return S_OK;
//}

HRESULT iterator_FindNextFile::OnAdaptorMethod_increment()
{
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_spRefCountedDataBlock );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( m_spRefCountedDataBlock->m_ohFindHandle.has_value() );

	// Copy buffer and clear internal to do operation; will copy back on success
	auto spResultDataBuffer = m_spResultDataBuffer;
	m_spResultDataBuffer = {};

	BOOL bSuccess = ::FindNextFile(
		m_spRefCountedDataBlock->m_ohFindHandle.value(),
		reinterpret_cast<WIN32_FIND_DATA*>(spResultDataBuffer.get()) );
	if (bSuccess)
	{
		m_spResultDataBuffer = spResultDataBuffer;
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

HRESULT iterator_FindNextFile::OnDestroy_FindClose( RefCountedDataBlock* pRefCountedDataBlock )
{
	if (!pRefCountedDataBlock)
	{
		return S_FALSE;
	}
	if (!pRefCountedDataBlock->m_ohFindHandle.has_value())
	{
		return S_FALSE;
	}

	auto oOnDestroy_ClearHandleVar = MakeActionOnDestruction( [&] { pRefCountedDataBlock->m_ohFindHandle = {}; } );

	BOOL bSuccess;

	bSuccess = ::FindClose(
		pRefCountedDataBlock->m_ohFindHandle.value() );
	ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( bSuccess );

	return S_OK;
}

class enum_files
{
public:
	vlr::tstring m_sSearchString;
	FINDEX_SEARCH_OPS m_dwSearchOps = FindExSearchNameMatch;
	DWORD m_dwAdditionalFlags = 0;

protected:
	HRESULT OnBegin( iterator_FindNextFile& iter ) const
	{
		auto spResultDataBuffer = std::shared_ptr<BYTE[]>{ new BYTE[sizeof( WIN32_FIND_DATA )] };

		auto hFindHandle = ::FindFirstFileEx(
			vlr::tzstring_view{ m_sSearchString },
			FindExInfoBasic,
			spResultDataBuffer.get(),
			m_dwSearchOps,
			nullptr,
			m_dwAdditionalFlags );
		if (hFindHandle == INVALID_HANDLE_VALUE)
		{
			iter.m_odwLastError = ::GetLastError();
			return S_FALSE;
		}

		auto spRefCountedDataBlock = std::shared_ptr<iterator_FindNextFile::RefCountedDataBlock>{ new iterator_FindNextFile::RefCountedDataBlock( hFindHandle ), &iterator_FindNextFile::OnDestroy_FindClose };
		ASSERT_NONZERO__OR_RETURN_EUNEXPECTED( spRefCountedDataBlock );
		iter.m_spRefCountedDataBlock = spRefCountedDataBlock;
		iter.m_spResultDataBuffer = spResultDataBuffer;

		return S_OK;
	}

public:
	inline auto begin() const
	{
		auto iter = iterator_FindNextFile{};
		OnBegin( iter );
		return iter;
	}
	inline auto end() const
	{
		return iterator_FindNextFile{};
	}
};

NAMESPACE_END //( filesystem )

NAMESPACE_END //( win32 )

NAMESPACE_END //( vlr )
