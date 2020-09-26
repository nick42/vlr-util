#include "pch.h"

#include "vlr/util.api_adaptor.OutString.h"

TEST( util_api_adaptor_OutString, general )
{
	vlr::tstring sValue = _T( "foo" );
	vlr::util::api_adaptor::COutString oParam{ sValue };
}
