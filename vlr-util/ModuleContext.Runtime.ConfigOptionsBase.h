#pragma once

#include <vector>

#include <boost/program_options.hpp>

#include "UtilMacros.Namespace.h"
#include "config.h"
#include "util.std_aliases.h"

#include "ModuleContext.Runtime.CommandLine.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ModuleContext )

NAMESPACE_BEGIN( Runtime )

class CConfigOptionsBase
{
public:
	HRESULT ParseOptions_CommandLine_withBoost(
		const vlr::ModuleContext::Runtime::CCommandLine& oCommandLine,
		const std::function<const boost::program_options::options_description& ()>& fGetRecognizedOptions );

public:
	std::vector<vlr::tstring> m_oUnrecognizedOptionsArray;

protected:
	HRESULT RecordUnrecognizedOptions( const std::vector<std::string>& oOptionsArray );
	HRESULT RecordUnrecognizedOptions( const std::vector<std::wstring>& oOptionsArray );

	struct OptionHandler_Basic
	{
		vlr::zstring_view m_svzOptionName;
		std::function<void( const boost::program_options::variable_value& oValue )> m_fOnOptionValue;

		OptionHandler_Basic(
			vlr::zstring_view svzOptionName,
			const std::function<void( const boost::program_options::variable_value& oValue )>& fOnOptionValue )
			: m_svzOptionName{ svzOptionName }
			, m_fOnOptionValue{ fOnOptionValue }
		{}
	};
	std::vector<OptionHandler_Basic> m_oRecordParsedOptionActionArray;

public:
	HRESULT AddOptionMapping_Basic_Flag(
		vlr::zstring_view svzOptionName,
		std::optional<bool>& obValue );
	HRESULT AddOptionMapping_Basic_StringValue(
		vlr::zstring_view svzOptionName,
		std::optional<vlr::tstring>& osValue );

protected:
	// Note: Default implementation uses m_oRecordParsedOptionActionArray to record values; 
	// populate collection or override virtual method.
	virtual HRESULT RecordParsedOptions(
		const boost::program_options::variables_map& oParsedVariablesMap );

};

NAMESPACE_END //( Runtime )

NAMESPACE_END //( ModuleContext )

NAMESPACE_END //( vlr )
