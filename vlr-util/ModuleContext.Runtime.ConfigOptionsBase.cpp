#include "pch.h"
#include "ModuleContext.Runtime.ConfigOptionsBase.h"

#if VLR_HASDEP_BOOST_PROGRAM_OPTIONS

#include "util.convert.StringConversion.h"
#include "UtilMacros.Assertions.h"

namespace vlr {

namespace ModuleContext {

namespace Runtime {

HRESULT CConfigOptionsBase::RecordUnrecognizedOptions( const std::vector<std::string>& oOptionsArray )
{
	m_oUnrecognizedOptionsArray.reserve( m_oUnrecognizedOptionsArray.size() + oOptionsArray.size() );

	for (const auto& sOption : oOptionsArray)
	{
		m_oUnrecognizedOptionsArray.push_back( vlr::util::Convert::ToStdString( sOption ) );
	}

	return S_OK;
}

HRESULT CConfigOptionsBase::RecordUnrecognizedOptions( const std::vector<std::wstring>& oOptionsArray )
{
	m_oUnrecognizedOptionsArray.reserve( m_oUnrecognizedOptionsArray.size() + oOptionsArray.size() );

	for (const auto& sOption : oOptionsArray)
	{
		m_oUnrecognizedOptionsArray.push_back( vlr::util::Convert::ToStdString( sOption ) );
	}

	return S_OK;
}

HRESULT CConfigOptionsBase::ParseOptions_CommandLine_withBoost(
	const vlr::ModuleContext::Runtime::CCommandLine& oCommandLine,
	const std::function<const boost::program_options::options_description& ()>& fGetRecognizedOptions )
{
	HRESULT hr;

	boost::program_options::variables_map oParsedVariablesMap;

	if (oCommandLine.m_ppArgArray_UTF16)
	{
		auto oParsedOptions = boost::program_options::basic_command_line_parser{ oCommandLine.m_nArgCount, oCommandLine.m_ppArgArray_UTF16 }
			.options( fGetRecognizedOptions() )
			.allow_unregistered()
			.run();
		boost::program_options::store( oParsedOptions, oParsedVariablesMap );

		auto oUnrecognizedOptionsArray = boost::program_options::collect_unrecognized( oParsedOptions.options, boost::program_options::include_positional );
		RecordUnrecognizedOptions( oUnrecognizedOptionsArray );
	}
	else if (oCommandLine.m_ppArgArray_ASCII)
	{
		auto oParsedOptions = boost::program_options::basic_command_line_parser{ oCommandLine.m_nArgCount, oCommandLine.m_ppArgArray_ASCII }
			.options( fGetRecognizedOptions() )
			.allow_unregistered()
			.run();
		boost::program_options::store( oParsedOptions, oParsedVariablesMap );

		auto oUnrecognizedOptionsArray = boost::program_options::collect_unrecognized( oParsedOptions.options, boost::program_options::include_positional );
		RecordUnrecognizedOptions( oUnrecognizedOptionsArray );
	}
	else
	{
		VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( E_UNEXPECTED );
	}

	boost::program_options::notify( oParsedVariablesMap );

	// Per example (we won't do here)...
	//if (oParsedVariablesMap.count( m_svzOptionName_Help ) > 0)
	//{
	//	std::cout << GetOptionsDesc_All() << "\n";
	//}

	hr = RecordParsedOptions( oParsedVariablesMap );
	VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_HRESULT( hr );

	return S_OK;
}

HRESULT CConfigOptionsBase::AddOptionMapping_Basic_Flag(
	vlr::zstring_view svzOptionName,
	std::optional<bool>& obValue )
{
	auto fAction = [svzOptionName, &obValue]( const boost::program_options::variable_value& /*oValue*/ )
	{
		// If we got a value, it means the flag was present
		obValue = true;
	};
	m_oRecordParsedOptionActionArray.emplace_back( svzOptionName, fAction );

	return S_OK;
}

HRESULT CConfigOptionsBase::AddOptionMapping_Basic_StringValue(
	vlr::zstring_view svzOptionName,
	std::optional<vlr::tstring>& osValue )
{
	auto fAction = [svzOptionName, &osValue]( const boost::program_options::variable_value& oValue )
	{
		osValue = vlr::util::Convert::ToStdString( oValue.as<std::string>() );
	};
	m_oRecordParsedOptionActionArray.emplace_back( svzOptionName, fAction );

	return S_OK;
}

HRESULT CConfigOptionsBase::RecordParsedOptions(
	const boost::program_options::variables_map& oParsedVariablesMap )
{
	for (const auto& oOptionHandler : m_oRecordParsedOptionActionArray)
	{
		VLR_ASSERT_NOTBLANK_OR_RETURN_EUNEXPECTED( oOptionHandler.m_svzOptionName );
		VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED( oOptionHandler.m_fOnOptionValue );

		if (oParsedVariablesMap.count(oOptionHandler.m_svzOptionName.toStdString()) == 0)
		{
			continue;
		}

		const auto& oOptionValue = oParsedVariablesMap[oOptionHandler.m_svzOptionName.toStdString()];
		oOptionHandler.m_fOnOptionValue( oOptionValue );
	}

	return S_OK;
}

} // namespace Runtime

} // namespace ModuleContext

} // namespace vlr

#endif // VLR_HASDEP_BOOST_PROGRAM_OPTIONS
