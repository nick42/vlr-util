#pragma once

#include "config.h"

#include "include.fmt.h"

#include "util.std_aliases.h"
#include "zstring_view.h"

#if VLR_CONFIG_LOG_CONTEXT_INCLUDE_FILE
#define VLR_LOG_CONTEXT_FILE _T(__FILE__)
#else
#define VLR_LOG_CONTEXT_FILE {}
#endif

#if VLR_CONFIG_LOG_CONTEXT_INCLUDE_LINE
#define VLR_LOG_CONTEXT_LINE __LINE__
#else
#define VLR_LOG_CONTEXT_LINE 0
#endif

#if VLR_CONFIG_LOG_CONTEXT_INCLUDE_FUNCTION
#define VLR_LOG_CONTEXT_FUNCTION _T(__FUNCTION__)
#else
#define VLR_LOG_CONTEXT_FUNCTION {}
#endif

namespace vlr {

namespace logging {

class CCodeContext
{
public:
	tzstring_view m_svzFile;
	int m_nLineNumber = 0;
	tzstring_view m_svzFunction;

public:
	static constexpr auto svzPathSeparators = tzstring_view{ _T("/\\") };
	constexpr tzstring_view GetFileNameOnly() const
	{
		auto nIndex = m_svzFile.find_last_of(svzPathSeparators);
		if (nIndex == m_svzFile.npos)
		{
			return m_svzFile;
		}
		return m_svzFile.trailing_end(nIndex + 1);
	}

	auto GetCodePositionIndicator() const -> vlr::tstring
	{
		if (m_svzFile.empty())
		{
			return _T("[unknown]");
		}
		return fmt::format(_T("{}:{}"), GetFileNameOnly().asStringView(), m_nLineNumber);
	}

	static constexpr CCodeContext FunctionOnly()
	{
		return CCodeContext{ {}, 0, VLR_LOG_CONTEXT_FUNCTION };
	}

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

#define VLR_CODE_CONTEXT vlr::logging::CCodeContext{ VLR_LOG_CONTEXT_FILE, VLR_LOG_CONTEXT_LINE, VLR_LOG_CONTEXT_FUNCTION }

namespace LogicalLevel {

enum ELogicalLevel : uint16_t
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

} // namespace LogicalLevel

class CMessageContext
{
public:
	CCodeContext m_oCodeContext;
	LogicalLevel::ELogicalLevel m_eLogicalLevel = LogicalLevel::Unknown;
	uint16_t m_nContextFlags = 0;

public:
	// Note: C++20 allows constexpr virtual destructors
	virtual ~CMessageContext() = default;
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

} // namespace logging

} // namespace vlr
