#include "pch.h"

#include "vlr/logging.MessageContext.h"

TEST( logging_MessageContext, general )
{
	{
		auto oMessageContext = vlr::logging::CMessageContext{};
		EXPECT_EQ( oMessageContext.m_oCodeContext.m_svzFile.empty(), true );
		EXPECT_EQ( oMessageContext.m_oCodeContext.m_nLineNumber, 0 );
		EXPECT_EQ( oMessageContext.m_oCodeContext.m_svzFunction.empty(), true );
	}
	{
		auto oMessageContext = vlr::logging::CMessageContext{ VLR_CODE_CONTEXT};
		EXPECT_NE( oMessageContext.m_oCodeContext.m_svzFile.empty(), true );
		EXPECT_NE( oMessageContext.m_oCodeContext.m_nLineNumber, 0 );
		EXPECT_NE( oMessageContext.m_oCodeContext.m_svzFunction.empty(), true );
	}
}
