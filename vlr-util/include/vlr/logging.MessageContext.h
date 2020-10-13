#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( logging )

class CCodeContext
{
public:
	tzstring_view m_svzFile;
	int m_nLineNumber = 0;
	tzstring_view m_svzFunction;

public:
	constexpr explicit CCodeContext() = default;
	constexpr CCodeContext(
		tzstring_view svzFile,
		int nLineNumber,
		tzstring_view svzFunction )
		: m_svzFile{ svzFile }
		, m_nLineNumber{ nLineNumber }
		, m_svzFunction{ svzFunction }
	{}
};

#define VLR_CODE_CONTEXT vlr::logging::CCodeContext{ _T(__FILE__), __LINE__, _T(__FUNCTION__) }

NAMESPACE_BEGIN( LogicalLevel )

enum ELogicalLevel
{
	Unknown,
	Debug,
	Trace,
	Verbose,
	Info,
	Warning,
	Error,
	Critical,
};

NAMESPACE_END //( LogicalLevel )

class CMessageContext
{
public:
	CCodeContext m_oCodeContext;
	LogicalLevel::ELogicalLevel m_eLogicalLevel = LogicalLevel::Unknown;

public:
	constexpr explicit CMessageContext() = default;
	constexpr CMessageContext( const CCodeContext& oCodeContext )
		: m_oCodeContext{ oCodeContext }
	{}
	constexpr CMessageContext(
		const CCodeContext& oCodeContext,
		LogicalLevel::ELogicalLevel eLogicalLevel )
		: m_oCodeContext{ oCodeContext }
		, m_eLogicalLevel{ eLogicalLevel }
	{}
};

#define VLR_LOG_CONTEXT_DEBUG vlr::logging::CMessageContext{ VLR_CODE_CONTEXT, vlr::logging::LogicalLevel::Debug }
#define VLR_LOG_CONTEXT_VERBOSE vlr::logging::CMessageContext{ VLR_CODE_CONTEXT, vlr::logging::LogicalLevel::Verbose }
#define VLR_LOG_CONTEXT_DEFAULT vlr::logging::CMessageContext{ VLR_CODE_CONTEXT, vlr::logging::LogicalLevel::Info }
#define VLR_LOG_CONTEXT_WARNING vlr::logging::CMessageContext{ VLR_CODE_CONTEXT, vlr::logging::LogicalLevel::Warning }

NAMESPACE_END //( logging )

NAMESPACE_END //( vlr )
