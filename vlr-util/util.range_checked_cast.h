#pragma once

#include "config.h"

#include "util.choice.h"
#include "util.static_assert.h"

#undef min
#undef max

namespace vlr {

namespace util {

namespace detail {

template< typename TDest, typename TSource >
constexpr bool SourceAlwaysFitsInDest()
{
	if constexpr (std::is_unsigned_v<TDest> && std::is_unsigned_v<TSource> && (sizeof(TDest) >= sizeof(TSource)))
	{
		return true;
	}
	if constexpr (std::is_signed_v<TDest> && std::is_signed_v<TSource> && (sizeof(TDest) >= sizeof(TSource)))
	{
		return true;
	}
	if constexpr (std::is_signed_v<TDest> && std::is_unsigned_v<TSource> && (sizeof(TDest) > sizeof(TSource)))
	{
		return true;
	}

	return false;
}

template< typename TDest, typename TSource, typename std::enable_if_t<detail::SourceAlwaysFitsInDest<TDest, TSource>()>* = nullptr >
constexpr auto range_checked_cast_choice(const TSource& nValue, choice<0>&&)
{
	return static_cast<TDest>(nValue);
}

template< typename TDest, typename TSource, typename std::enable_if_t<!detail::SourceAlwaysFitsInDest<TDest, TSource>()>* = nullptr >
inline constexpr auto range_checked_cast_choice(TSource nValue, choice<1>&&)
{
	// Requires runtime checking

	auto fGetInRangeResult = [](TSource nValue)
	{
		return static_cast<TDest>(nValue);
	};
	auto fGetOutOfRangeResult = [](TSource /*nValue*/)
	{
		ASSERT(0);
		return TDest{};
	};

	if constexpr (std::is_unsigned_v<TDest>)
	{
		if constexpr (std::is_unsigned_v<TSource>)
		{
			// This should only be called if the dest size is strictly smaller; if the source was smaller or equal, then 
			// this should have been handled in the external function.
			static_assert(sizeof(TDest) < sizeof(TSource));

			// Handle possible overflow
			auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
			if (nValue > nMaxDest_AsSource)
			{
				return fGetOutOfRangeResult(nValue);
			}

			return fGetInRangeResult(nValue);
		}
		else // std::is_signed_v<TSource>
		{
			if (nValue < 0)
			{
				return fGetOutOfRangeResult(nValue);
			}

			if constexpr (sizeof(TDest) >= sizeof(TSource))
			{
				// Will always fit at this point
				return fGetInRangeResult(nValue);
			}
			else
			{
				// Possible overflow
				auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
				if (nValue > nMaxDest_AsSource)
				{
					return fGetOutOfRangeResult(nValue);
				}

				return fGetInRangeResult(nValue);
			}
		}
	}
	else // std::is_signed_v<TDest>
	{
		if constexpr (std::is_unsigned_v<TSource>)
		{
			// This should only be called if the dest size is smaller or equal; if the source was smaller, this should have been 
			// handled in the external function.
			static_assert(sizeof(TDest) <= sizeof(TSource));

			// Handle possible overflow
			auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
			if (nValue > nMaxDest_AsSource)
			{
				return fGetOutOfRangeResult(nValue);
			}

			return fGetInRangeResult(nValue);
		}
		else // std::is_signed_v<TSource>
		{
			// This should only be called if the dest size is strictly smaller; if the source was smaller or equal, then 
			// this should have been handled in the external function.
			static_assert(sizeof(TDest) < sizeof(TSource));

			// Handle possible underflow or overflow
			auto nMinDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().min());
			if (nValue < nMinDest_AsSource)
			{
				return fGetOutOfRangeResult(nValue);
			}
			auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
			if (nValue > nMaxDest_AsSource)
			{
				return fGetOutOfRangeResult(nValue);
			}

			return fGetInRangeResult(nValue);
		}
	}
}

} // namespace detail

template< typename TDest, typename TSource >
constexpr auto range_checked_cast(TSource nValue)
{
	return detail::range_checked_cast_choice<TDest>(nValue, choice<0>{});
}

} // namespace util

} // namespace vlr
