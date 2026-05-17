#include "pch.h"

#include "vlr-util/AutoFreeResource.h"

TEST( CAutoFreeResource, explicit_construct )
{
	auto pBuffer = new char[100];
	auto fFreeBuffer = []( char*& pBuffer )
	{
		free( pBuffer );
	};
	auto oAutoFreeBuffer = vlr::CAutoFreeResource<char*, void>{ pBuffer, fFreeBuffer };
}

TEST( CActionOnDestruction, MakeActionOnDestruction )
{
	auto pBuffer = new char[100];
	auto oAutoFreeBuffer = vlr::MakeActionOnDestruction( [&]
		{
			free( pBuffer );
		} );
}
