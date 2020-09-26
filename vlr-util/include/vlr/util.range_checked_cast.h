#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "vlr/util.choice.h"

#undef min
#undef max

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

NAMESPACE_BEGIN( detail )

template< typename TDest, typename TSource >
constexpr bool SourceAlwaysFitsInDest()
{
	if constexpr (std::is_unsigned_v<TDest> && std::is_unsigned_v<TSource> && (sizeof( TDest ) >= sizeof( TSource )))
	{
		return true;
	}
	if constexpr (std::is_signed_v<TDest> && std::is_signed_v<TSource> && (sizeof( TDest ) >= sizeof( TSource )))
	{
		return true;
	}
	if constexpr (std::is_signed_v<TDest> && std::is_unsigned_v<TSource> && (sizeof( TDest ) > sizeof( TSource )))
	{
		return true;
	}

	return false;
}

template< typename TDest, typename TSource, typename std::enable_if_t<detail::SourceAlwaysFitsInDest<TDest, TSource>()>* = nullptr >
constexpr auto range_checked_cast_choice( const TSource& nValue, choice<0>&& )
{
	return static_cast<TDest>(nValue);
}

template< typename TDest, typename TSource, typename std::enable_if_t<!detail::SourceAlwaysFitsInDest<TDest, TSource>()>* = nullptr >
inline auto range_checked_cast_choice( TSource nValue, choice<1>&& )
{
	// Requires runtime checking

	auto fGetInRangeResult = []( TSource nValue )
	{
		return static_cast<TDest>(nValue);
	};
	auto fGetOutOfRangeResult = []( TSource /*nValue*/ )
	{
		ASSERT( 0 );
		return TDest{};
	};

	if constexpr (std::is_unsigned_v<TDest>)
	{
		if constexpr (std::is_unsigned_v<TSource>)
		{
			if constexpr (sizeof( TDest ) < sizeof( TSource ))
			{
				// Possible overflow
				auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
				if (nValue > nMaxDest_AsSource)
				{
					return fGetOutOfRangeResult( nValue );
				}

				return fGetInRangeResult( nValue );
			}
			else
			{
				// This should always fit; should not call here
				static_assert(false);
			}
		}
		else // std::is_signed_v<TSource>
		{
			if (nValue < 0)
			{
				return fGetOutOfRangeResult( nValue );
			}

			if constexpr (sizeof( TDest ) >= sizeof( TSource ))
			{
				// Will always fit at this point
				return fGetInRangeResult( nValue );
			}
			else
			{
				// Possible overflow
				auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
				if (nValue > nMaxDest_AsSource)
				{
					return fGetOutOfRangeResult( nValue );
				}

				return fGetInRangeResult( nValue );
			}
		}
	}
	else // std::is_signed_v<TDest>
	{
		if constexpr (std::is_unsigned_v<TSource>)
		{
			if constexpr (sizeof( TDest ) <= sizeof( TSource ))
			{
				// Possible overflow
				auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
				if (nValue > nMaxDest_AsSource)
				{
					return fGetOutOfRangeResult( nValue );
				}

				return fGetInRangeResult( nValue );
			}
			else
			{
				// This should always fit; should not call here
				static_assert(false);
			}
		}
		else // std::is_signed_v<TSource>
		{
			if constexpr (sizeof( TDest ) >= sizeof( TSource ))
			{
				// This should always fit; should not call here
				static_assert(false);
			}
			else
			{
				// Possible underflow or overflow
				auto nMinDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().min());
				if (nValue < nMinDest_AsSource)
				{
					return fGetOutOfRangeResult( nValue );
				}
				auto nMaxDest_AsSource = static_cast<TSource>(std::numeric_limits<TDest>().max());
				if (nValue > nMaxDest_AsSource)
				{
					return fGetOutOfRangeResult( nValue );
				}

				return fGetInRangeResult( nValue );
			}
		}
	}
}

NAMESPACE_END //( detail )

template< typename TDest, typename TSource >
inline auto range_checked_cast( TSource nValue )
{
	return detail::range_checked_cast_choice<TDest>( nValue, choice<0>{} );
}

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
