#include "pch.h"

#include "vlr-util/util.choice.h"

template< typename TParam, typename std::enable_if_t<std::is_same_v<TParam, int>>* = nullptr >
inline auto GetValue_choice( TParam tParam, vlr::util::choice<0>&& )
{
	return tParam;
}

template< typename TParam, typename std::enable_if_t<std::is_same_v<TParam, double>>* = nullptr >
inline auto GetValue_choice( TParam tParam, vlr::util::choice<1>&& )
{
	return tParam;
}

template< typename TParam >
inline auto GetValue( TParam tParam )
{
	return GetValue_choice( tParam, vlr::util::choice<0>{} );
}

TEST( util_choice, general )
{
	EXPECT_EQ( GetValue( int{ 1 } ), 1 );
	EXPECT_EQ( GetValue( double{ 1.1 } ), 1.1 );
}
