#pragma once

#include <optional>

#include <boost/iterator/iterator_facade.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.IsBitValue.h"

VLR_NAMESPACE_BEGIN( vlr )

VLR_NAMESPACE_BEGIN( enums )

// Sequential values iterator

template< typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 0, typename TDerefType = const TNumericType& >
class sequential_range_iterator
	: public boost::iterator_facade<sequential_range_iterator<TNumericType, tMaxValue, tMinValue, TDerefType>, TNumericType const, boost::bidirectional_traversal_tag, TDerefType>
{
protected:
	friend class boost::iterator_core_access;

private:
	using this_type = sequential_range_iterator<TNumericType, tMaxValue, tMinValue, TDerefType>;

public:
	static constexpr auto GetMinValue()
	{
		return tMinValue;
	}
	static constexpr auto GetMaxValue()
	{
		return tMaxValue;
	}
	static constexpr auto IsValueInRange( TNumericType nValue )
	{
		return true
			&& (nValue >= tMinValue)
			&& (nValue <= tMaxValue)
			;
	}

protected:
	std::optional<TNumericType> m_onValue;

protected:
	constexpr void increment() noexcept
	{
		if (!m_onValue.has_value())
		{
			return;
		}
		auto& nValue = m_onValue.value();

		if (nValue >= tMaxValue)
		{
			m_onValue = {};
			return;
		}

		++nValue;
	}
	constexpr void decrement() noexcept
	{
		if (!m_onValue.has_value())
		{
			return;
		}
		auto& nValue = m_onValue.value();

		if (nValue <= tMinValue)
		{
			m_onValue = {};
			return;
		}

		--nValue;
	}

	constexpr bool equal( const this_type& oOther ) const noexcept
	{
		return true
			&& (m_onValue == oOther.m_onValue)
			;
	}

	constexpr TDerefType dereference() const
	{
		ASSERT( m_onValue.has_value() );

		// If we're returning a reference, return the reference to the actual value, else return a cast

		if constexpr (std::is_reference_v<TDerefType>)
		{
			return m_onValue.value();
		}
		else
		{
			return static_cast<TDerefType>(m_onValue.value());
		}
	}

public:
	constexpr sequential_range_iterator() noexcept = default;
	explicit constexpr sequential_range_iterator( TNumericType nValue ) noexcept
		: m_onValue{ nValue }
	{
		static_assert(tMaxValue >= tMinValue);
		// TODO? static_assert that TNumericType is actually numeric?

		if (nValue < tMinValue)
		{
			m_onValue = {};
		}
		else if (nValue >= tMaxValue)
		{
			m_onValue = {};
		}
	}
};

template< typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 0, typename TDerefType = TNumericType const& >
class CRangeInfoSequentialT
{
private:
	using this_type = CRangeInfoSequentialT<TNumericType, tMaxValue, tMinValue, TDerefType>;
	using iterator = sequential_range_iterator<TNumericType, tMaxValue, tMinValue, TDerefType>;

public:
	static constexpr auto m_tMinValue = tMinValue;
	static constexpr auto m_tMaxValue = tMaxValue;

	static constexpr bool IsSequential()
	{
		return true;
	}
	static constexpr auto GetMinValue()
	{
		return iterator::GetMinValue();
	}
	static constexpr auto GetMaxValue()
	{
		return iterator::GetMaxValue();
	}
	static constexpr auto IsValueInRange( TNumericType nValue )
	{
		return iterator::IsValueInRange( nValue );
	}

public:
	constexpr auto begin() const
	{
		return iterator{ m_tMinValue };
	}
	constexpr auto end() const
	{
		return iterator{};
	}
	constexpr auto rbegin() const
	{
		return iterator{ m_tMaxValue };
	}
	constexpr auto rend() const
	{
		return iterator{};
	}

public:
	constexpr CRangeInfoSequentialT() noexcept = default;
};

template< DWORD_PTR nMaxValue, DWORD_PTR nMinValue = 0, typename TDerefType = const DWORD_PTR& >
class CRangeInfoSequential_DWORD_PTR
	: public CRangeInfoSequentialT<DWORD_PTR, nMaxValue, nMinValue, TDerefType>
{
private:
	using this_type = CRangeInfoSequential_DWORD_PTR<nMaxValue, nMinValue, TDerefType>;
	using BaseClass = CRangeInfoSequentialT<DWORD_PTR, nMaxValue, nMinValue, TDerefType>;

public:
	using BaseClass::begin;
	using BaseClass::end;
};

template< INT_PTR nMaxValue, INT_PTR nMinValue = 0, typename TDerefType = const INT_PTR& >
class CRangeInfoSequential_INT_PTR
	: public CRangeInfoSequentialT<INT_PTR, nMaxValue, nMinValue, TDerefType>
{
private:
	using this_type = CRangeInfoSequential_INT_PTR<nMaxValue, nMinValue, TDerefType>;
	using BaseClass = CRangeInfoSequentialT<INT_PTR, nMaxValue, nMinValue, TDerefType>;

public:
	using BaseClass::begin;
	using BaseClass::end;
};

template< DWORD_PTR nMaxValue, DWORD_PTR nMinValue = 0 >
using CRangeInfoSequential_Unsigned_Default = CRangeInfoSequential_DWORD_PTR<nMaxValue, nMinValue>;

template< INT_PTR nMaxValue, INT_PTR nMinValue = 0 >
using CRangeInfoSequential_Signed_Default = CRangeInfoSequential_DWORD_PTR<nMaxValue, nMinValue>;

// Bitmask iterator

template< typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 1, typename TDerefType = const TNumericType&, typename std::enable_if_t<vlr::util::IsSingleBitValue(tMaxValue)>* = nullptr >
class bitmask_range_iterator
	: public boost::iterator_facade<bitmask_range_iterator<TNumericType, tMaxValue, tMinValue, TNumericType>, TNumericType const, boost::bidirectional_traversal_tag>
{
protected:
	friend class boost::iterator_core_access;

private:
	using this_type = bitmask_range_iterator<TNumericType, tMaxValue, tMinValue, TNumericType>;

public:
	static constexpr auto GetMinValue()
	{
		return tMinValue;
	}
	static constexpr auto GetMaxValue()
	{
		return tMaxValue;
	}
	// Note: Only single bit values are in range for a bitmask iterator
	static constexpr auto IsValueInRange( TNumericType nValue )
	{
		return true
			&& (nValue >= tMinValue)
			&& (nValue <= tMaxValue)
			&& (vlr::util::IsSingleBitValue( nValue ))
			;
	}

protected:
	std::optional<TNumericType> m_onValue;

protected:
	constexpr void increment() noexcept
	{
		if (!m_onValue.has_value())
		{
			return;
		}
		auto& nValue = m_onValue.value();

		if (nValue >= tMaxValue)
		{
			m_onValue = {};
			return;
		}

		nValue <<= 1;
	}
	constexpr void decrement() noexcept
	{
		if (!m_onValue.has_value())
		{
			return;
		}
		auto& nValue = m_onValue.value();

		if (nValue <= tMinValue)
		{
			m_onValue = {};
			return;
		}

		nValue >>= 1;
	}

	constexpr bool equal( const this_type& oOther ) const noexcept
	{
		return true
			&& (m_onValue == oOther.m_onValue)
			;
	}

	constexpr TDerefType dereference() const
	{
		ASSERT( m_onValue.has_value() );

		// If we're returning a reference, return the reference to the actual value, else return a cast

		if constexpr (std::is_reference_v<TDerefType>)
		{
			return m_onValue.value();
		}
		else
		{
			return static_cast<TDerefType>(m_onValue.value());
		}
	}

public:
	constexpr bitmask_range_iterator() noexcept = default;
	explicit constexpr bitmask_range_iterator( TNumericType nValue ) noexcept
		: m_onValue{ nValue }
	{
		static_assert(tMaxValue >= tMinValue);
		// TODO? static_assert that TNumericType is actually numeric?

		if (!vlr::util::IsSingleBitValue( nValue ))
		{
			m_onValue = {};
		}
		else if (nValue < tMinValue)
		{
			m_onValue = {};
		}
		else if (nValue >= tMaxValue)
		{
			m_onValue = {};
		}
	}
};

template< typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 1, typename TDerefType = TNumericType const& >
class CRangeInfoBitmaskT
{
private:
	using this_type = CRangeInfoBitmaskT<TNumericType, tMaxValue, tMinValue, TDerefType>;
	using iterator = bitmask_range_iterator<TNumericType, tMaxValue, tMinValue, TDerefType>;

public:
	static constexpr auto m_tMinValue = tMinValue;
	static constexpr auto m_tMaxValue = tMaxValue;
	// Note: This is platform-specific; width of char is not always 8
	static constexpr auto m_nBitmaskWidth = sizeof( TNumericType ) * 8;

	static constexpr bool IsSequential()
	{
		return false;
	}
	static constexpr auto GetMinValue()
	{
		return iterator::GetMinValue();
	}
	static constexpr auto GetMaxValue()
	{
		return iterator::GetMaxValue();
	}
	static constexpr auto IsValueInRange( TNumericType nValue )
	{
		return iterator::IsValueInRange( nValue );
	}

public:
	constexpr auto begin() const
	{
		return iterator{ m_tMinValue };
	}
	constexpr auto end() const
	{
		return iterator{};
	}
	constexpr auto rbegin() const
	{
		return iterator{ m_tMaxValue };
	}
	constexpr auto rend() const
	{
		return iterator{};
	}

public:
	constexpr CRangeInfoBitmaskT() noexcept = default;
};

template< DWORD_PTR nMaxValue, DWORD_PTR nMinValue = 1, typename TDerefType = const DWORD_PTR& >
class CRangeInfoBitmask_DWORD_PTR
	: public CRangeInfoBitmaskT<DWORD_PTR, nMaxValue, nMinValue, TDerefType>
{
private:
	using this_type = CRangeInfoBitmask_DWORD_PTR<nMaxValue, nMinValue, TDerefType>;
	using BaseClass = CRangeInfoBitmaskT<DWORD_PTR, nMaxValue, nMinValue, TDerefType>;

public:
	using BaseClass::begin;
	using BaseClass::end;
};

template< DWORD_PTR nMaxValue, DWORD_PTR nMinValue = 1 >
using CRangeInfoBitmask_Unsigned_Default = CRangeInfoSequential_DWORD_PTR<nMaxValue, nMinValue>;

VLR_NAMESPACE_END //( enums )

VLR_NAMESPACE_END //( vlr )
