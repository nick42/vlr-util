#include "pch.h"

#include "vlr-util/logging.LogMessage.h"

TEST( logging_LogMessage, general )
{
	using namespace vlr::logging;

	LogMessage( VLR_LOG_CONTEXT_DEFAULT, "Hello world!");
	LogMessage( VLR_LOG_CONTEXT_DEFAULT, L"Hello world!" );
	LogMessagePF( VLR_LOG_CONTEXT_DEFAULT, "Hello %s!", "world" );
	LogMessagePF( VLR_LOG_CONTEXT_DEFAULT, L"Hello %s!", L"world" );
	LogMessagePF( VLR_LOG_CONTEXT_DEFAULT, _T( "Hello %s!" ), _T( "world" ) );
	//LogMessagePF( VLR_LOG_CONTEXT_DEFAULT, "Hello %ls?", L"world" );
	//LogMessagePF( VLR_LOG_CONTEXT_DEFAULT, L"Hello %hs?", "world" );
}
