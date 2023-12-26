#pragma once

#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.IsBitValue.h"

namespace vlr {

namespace enums {

// Sequential values iterator

template <typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 0, typename TDerefType = const TNumericType&>
class SequentialRangeIterator
{
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = TNumericType;
	using difference_type = std::ptrdiff_t;
	using pointer = TNumericType*;
	using reference = TDerefType;

protected:
	bool m_bIsValid = false;
	TNumericType m_tCurrent{};

public:
	static constexpr auto GetMinValue()
	{
		return tMinValue;
	}
	static constexpr auto GetMaxValue()
	{
		return tMaxValue;
	}
	static constexpr auto IsValueInRange(TNumericType nValue)
	{
		return true
			&& (nValue >= tMinValue)
			&& (nValue <= tMaxValue)
			;
	}

	TNumericType operator*() const
	{
		if (!m_bIsValid)
		{
			throw std::exception("Invalid iterator");
		}
		return m_tCurrent;
	}

	SequentialRangeIterator& operator++()
	{
		if (m_tCurrent == tMaxValue)
		{
			m_bIsValid = false;
			m_tCurrent = {};
			return *this;
		}
		++m_tCurrent;
		return *this;
	}

	SequentialRangeIterator operator++(int)
	{
		SequentialRangeIterator temp = *this;
		++(*this);
		return temp;
	}

	SequentialRangeIterator& operator--()
	{
		if (m_tCurrent == tMinValue)
		{
			m_bIsValid = false;
			m_tCurrent = {};
			return *this;
		}
		--m_tCurrent;
		return *this;
	}

	SequentialRangeIterator operator--(int)
	{
		SequentialRangeIterator temp = *this;
		--(*this);
		return temp;
	}

	bool operator==(const SequentialRangeIterator& other) const
	{
		return true
			&& (m_bIsValid == other.m_bIsValid)
			&& (m_tCurrent == other.m_tCurrent);
	}

	bool operator!=(const SequentialRangeIterator& other) const
	{
		return !(*this == other);
	}

public:
	explicit SequentialRangeIterator() = default;
	explicit SequentialRangeIterator(TNumericType tCurrent)
		: m_bIsValid{ true }
		, m_tCurrent{ tCurrent }
	{}
};

template <typename T, T Min, T Max>
class SequentialRange {
public:
	using iterator = SequentialRangeIterator<T, Min, Max>;

	iterator begin() {
		return iterator(Min);
	}

	iterator end() {
		return iterator(Max + 1);
	}
};


template< typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 0, typename TDerefType = TNumericType const& >
class CRangeInfoSequentialT
{
private:
	using this_type = CRangeInfoSequentialT<TNumericType, tMaxValue, tMinValue, TDerefType>;
	using iterator = SequentialRangeIterator<TNumericType, tMaxValue, tMinValue, TDerefType>;

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

template <typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 0, typename std::enable_if_t<vlr::util::IsSingleBitValue(tMaxValue)>* = nullptr>
class BitmaskRangeIterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = TNumericType;
	using difference_type = std::ptrdiff_t;
	using pointer = TNumericType*;
	using reference = TNumericType&;

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
	static constexpr auto IsValueInRange(TNumericType nValue)
	{
		return true
			&& (nValue >= tMinValue)
			&& (nValue <= tMaxValue)
			&& (vlr::util::IsSingleBitValue(nValue))
			;
	}

protected:
	bool m_bIsValid = false;
	TNumericType m_tCurrent{};

public:
	TNumericType operator*() const {
		if (!m_bIsValid)
		{
			throw std::exception("Invalid iterator");
		}
		return m_tCurrent;
	}

	BitmaskRangeIterator& operator++()
	{
		if (m_tCurrent == tMaxValue)
		{
			m_bIsValid = false;
			m_tCurrent = {};
			return *this;
		}
		m_tCurrent = m_tCurrent << 1;
		return *this;
	}

	BitmaskRangeIterator operator++(int)
	{
		SequentialRangeIterator temp = *this;
		++(*this);
		return temp;
	}

	BitmaskRangeIterator& operator--()
	{
		if (m_tCurrent == tMinValue)
		{
			m_bIsValid = false;
			m_tCurrent = {};
			return *this;
		}
		m_tCurrent = m_tCurrent >> 1;
		return *this;
	}

	BitmaskRangeIterator operator--(int)
	{
		SequentialRangeIterator temp = *this;
		--(*this);
		return temp;
	}

	bool operator==(const BitmaskRangeIterator& other) const
	{
		return true
			&& (m_bIsValid == other.m_bIsValid)
			&& (m_tCurrent == other.m_tCurrent);
	}

	bool operator!=(const BitmaskRangeIterator& other) const
	{
		return !(*this == other);
	}

public:
	explicit BitmaskRangeIterator() = default;
	explicit BitmaskRangeIterator(TNumericType tCurrent)
		: m_bIsValid{ true }
		, m_tCurrent{ tCurrent }
	{}
};

template< typename TNumericType, TNumericType tMaxValue, TNumericType tMinValue = 1 >
class CRangeInfoBitmaskT
{
private:
	using this_type = CRangeInfoBitmaskT<TNumericType, tMaxValue, tMinValue>;
	using iterator = BitmaskRangeIterator<TNumericType, tMaxValue, tMinValue>;

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

template< DWORD_PTR nMaxValue, DWORD_PTR nMinValue = 1 >
class CRangeInfoBitmask_DWORD_PTR
	: public CRangeInfoBitmaskT<DWORD_PTR, nMaxValue, nMinValue>
{
private:
	using this_type = CRangeInfoBitmask_DWORD_PTR<nMaxValue, nMinValue>;
	using BaseClass = CRangeInfoBitmaskT<DWORD_PTR, nMaxValue, nMinValue>;

public:
	using BaseClass::begin;
	using BaseClass::end;
};

template< DWORD_PTR nMaxValue, DWORD_PTR nMinValue = 1 >
using CRangeInfoBitmask_Unsigned_Default = CRangeInfoSequential_DWORD_PTR<nMaxValue, nMinValue>;

} // namespace enums

} // namespace vlr
