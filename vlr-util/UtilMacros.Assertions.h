#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "AssertionHandling_Config.h"
#include "logging.MessageContext.h"
#include "zstring_view.h"
#include "util.IsNonZero.h"
#include "util.IsNotBlank.h"
#include "util.Result.h"

namespace vlr {

namespace detail {

template <typename TResult>
constexpr bool ResultIsSuccess(const TResult& tResult)
{
	if constexpr (std::is_same_v<std::decay_t<TResult>, SResult>)
	{
		return tResult.isSuccess();
	}
	else
	{
		// Note: This replicates SUCCEEDED in Windows
		return (((HRESULT)(tResult)) >= 0);
	}
}

} // namespace detail

} // namespace vlr

#ifdef VLR_CONFIG_ASSERTIONS_INCLUDE_FUNCTION
#define VLR_ASSERTION_FUNCTION_NAME _T("[") _T(__FUNCTION__) _T("] ")
#else
#define VLR_ASSERTION_FUNCTION_NAME
#endif

#ifdef VLR_CONFIG_ASSERTIONS_FAILURE_MESSAGE_PREFIX
#define VLR_ASSERTION_FAILURE_MESSAGE_PREFIX VLR_CONFIG_ASSERTIONS_FAILURE_MESSAGE_PREFIX
#else
#define VLR_ASSERTION_FAILURE_MESSAGE_PREFIX _T("Assertion failed; ")
#endif

#define VLR_ASSERTIONS_EVALUATE_COMPARISON( lhs, op, rhs ) ( (lhs) op (rhs) )

// Note: Assuming string literal at this point, so preprocessor concat works
// TODO: Convert to formatting, if/when we have the applicable library dependency

#define VLR_ASSERTIONS_HANDLE_CHECK_FAILURE( pcszFailureMessage ) \
	vlr::assert::HandleCheckFailure( VLR_CODE_CONTEXT, VLR_ASSERTION_FUNCTION_NAME VLR_ASSERTION_FAILURE_MESSAGE_PREFIX pcszFailureMessage );

#define VLR_ASSERTIONS_HANDLE_FAILURE_NONZERO( pcszValueName ) \
	VLR_ASSERTIONS_HANDLE_CHECK_FAILURE( _T("nonzero value: ") pcszValueName );

#define VLR_ASSERTIONS_HANDLE_FAILURE_NONTBLANK( pcszValueName ) \
	VLR_ASSERTIONS_HANDLE_CHECK_FAILURE( _T("notblank value: ") pcszValueName );

#define VLR_ASSERTIONS_HANDLE_FAILURE_COMPARE( lhs, op, rhs ) \
	VLR_ASSERTIONS_HANDLE_CHECK_FAILURE( _T("compare failure: ") _T(#lhs) _T(#op) _T(#rhs) );

#define VLR_ASSERTIONS_RETURN_NULL return;

#define VLR_ASSERTIONS_RETURN_EXPRESSION( expression ) return (expression);

#define VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( expression ) \
{ \
	vlr::assert::HandleCheckFailure( VLR_CODE_CONTEXT, VLR_ASSERTION_FUNCTION_NAME _T("Assertion failed (general)") ); \
	VLR_ASSERTIONS_RETURN_EXPRESSION( expression ) \
}

#define VLR_ASSERT_NONZERO_OR_RETURN( value ) \
{ \
	auto&& _value = (value); \
	if (vlr::util::IsNonZero( _value )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_NONZERO( _T(#value) ) \
		VLR_ASSERTIONS_RETURN_NULL \
	} \
}

#define VLR_ASSERT_NOTBLANK_OR_RETURN( value ) \
{ \
	auto&& _value = (value); \
	if (vlr::util::IsNotBlank( _value )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_NONTBLANK( _T(#value) ) \
		VLR_ASSERTIONS_RETURN_NULL \
	} \
}

#define VLR_ASSERT_COMPARE_OR_RETURN( lhs, op, rhs ) \
{ \
	auto&& _compareResult = VLR_ASSERTIONS_EVALUATE_COMPARISON( lhs, op, rhs ); \
	if (vlr::util::IsNonZero( _compareResult )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_COMPARE( lhs, op, rhs ) \
		VLR_ASSERTIONS_RETURN_NULL \
	} \
}

#define VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, expression ) \
{ \
	auto&& _value = (value); \
	if (vlr::util::IsNonZero( _value )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_NONZERO( _T(#value) ) \
		VLR_ASSERTIONS_RETURN_EXPRESSION( expression ) \
	} \
}

#define VLR_ASSERT_COMPARE_OR_CONTINUE( lhs, op, rhs ) \
{ \
	auto&& _compareResult = VLR_ASSERTIONS_EVALUATE_COMPARISON( lhs, op, rhs ); \
	if (vlr::util::IsNonZero( _compareResult )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_COMPARE( lhs, op, rhs ) \
	} \
}

#define VLR_ASSERT_NOTBLANK_OR_RETURN_EXPRESSION( value, expression ) \
{ \
	auto&& _value = (value); \
	if (vlr::util::IsNotBlank( _value )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_NONTBLANK( _T(#value) ) \
		VLR_ASSERTIONS_RETURN_EXPRESSION( expression ) \
	} \
}

#define VLR_ASSERT_COMPARE_OR_RETURN_EXPRESSION( lhs, op, rhs, expression ) \
{ \
	auto&& _compareResult = VLR_ASSERTIONS_EVALUATE_COMPARISON( lhs, op, rhs ); \
	if (vlr::util::IsNonZero( _compareResult )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_COMPARE( lhs, op, rhs ) \
		VLR_ASSERTIONS_RETURN_EXPRESSION( expression ) \
	} \
}

#define VLR_ASSERT_NONZERO_OR_CONTINUE( value ) \
{ \
	auto&& _value = (value); \
	if (vlr::util::IsNonZero( _value )) \
	{} \
	else \
	{ \
		VLR_ASSERTIONS_HANDLE_FAILURE_NONZERO( _T(#value) ) \
	} \
}

#define VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_FAILURE_VALUE VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( _tFailureValue )

#define VLR_ASSERT_NONZERO_OR_RETURN_FAILURE_VALUE( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, _tFailureValue )
#define VLR_ASSERT_NOTBLANK_OR_RETURN_FAILURE_VALUE( value ) VLR_ASSERT_NOTBLANK_OR_RETURN_EXPRESSION( value, _tFailureValue )
#define VLR_ASSERT_COMPARE_OR_RETURN_FAILURE_VALUE( lhs, op, rhs ) VLR_ASSERT_COMPARE_OR_RETURN_EXPRESSION( lhs, op, rhs, _tFailureValue )

#define VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, E_UNEXPECTED )
#define VLR_ASSERT_NOTBLANK_OR_RETURN_EUNEXPECTED( value ) VLR_ASSERT_NOTBLANK_OR_RETURN_EXPRESSION( value, E_UNEXPECTED )
#define VLR_ASSERT_COMPARE_OR_RETURN_EUNEXPECTED( lhs, op, rhs ) VLR_ASSERT_COMPARE_OR_RETURN_EXPRESSION( lhs, op, rhs, E_UNEXPECTED )

#define VLR_ASSERT_NONZERO_OR_RETURN_HRESULT_LAST_ERROR( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, HRESULT_FROM_WIN32( ::GetLastError() ) )
#define VLR_ASSERT_NOTBLANK_OR_RETURN_HRESULT_LAST_ERROR( value ) VLR_ASSERT_NOTBLANK_OR_RETURN_EXPRESSION( value, HRESULT_FROM_WIN32( ::GetLastError() ) )
#define VLR_ASSERT_COMPARE_OR_RETURN_HRESULT_LAST_ERROR( lhs, op, rhs ) VLR_ASSERT_COMPARE_OR_RETURN_EXPRESSION( lhs, op, rhs, HRESULT_FROM_WIN32( ::GetLastError() ) )

#define VLR_ASSERT_SUCCEEDED_OR_RETURN_RESULT( result ) { auto&& _result = (result); VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( vlr::detail::ResultIsSuccess(_result), _result ) }
#define VLR_ASSERT_SUCCEEDED_OR_RETURN_FAILURE_VALUE( result ) { auto&& _result = (result); VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( vlr::detail::ResultIsSuccess(_result), _tFailureValue ) }
#define VLR_ASSERT_SUCCEEDED_OR_CONTINUE( result ) { auto&& _result = (result); VLR_ASSERT_NONZERO_OR_CONTINUE( vlr::detail::ResultIsSuccess(_result) ) }

#define VLR_ON_SUCCESS_RETURN_VALUE( result ) { auto&& _result = (result); if (!vlr::detail::ResultIsSuccess(_result)) {} else { return _result; } }
#define VLR_ON_ERROR_RETURN_VALUE( result ) { auto&& _result = (result); if (vlr::detail::ResultIsSuccess(_result)) {} else { return _result; } }

#define VLR_ON_SPECIFIC_RESULT_RETURN_VALUE( result, target_result ) { auto&& _result = (result); if (_result != target_result) {} else { return _result; } }
#define VLR_ON_NOT_SPECIFIC_RESULT_RETURN_VALUE( result, target_result ) { auto&& _result = (result); if (_result == target_result) {} else { return _result; } }

// Per result type macros; to be deprecated at some point

#define VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_HRESULT( hr ) VLR_ASSERT_SUCCEEDED_OR_RETURN_RESULT( hr )
#define VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_FAILURE_VALUE( hr ) VLR_ASSERT_SUCCEEDED_OR_RETURN_FAILURE_VALUE( hr )
#define VLR_ASSERT_HR_SUCCEEDED_OR_CONTINUE( hr ) VLR_ASSERT_SUCCEEDED_OR_CONTINUE( hr )

#define VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT( sr ) VLR_ASSERT_SUCCEEDED_OR_RETURN_RESULT( sr )
#define VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_FAILURE_VALUE( sr ) VLR_ASSERT_SUCCEEDED_OR_RETURN_FAILURE_VALUE( sr )
#define VLR_ASSERT_SR_SUCCEEDED_OR_CONTINUE( sr ) VLR_ASSERT_SUCCEEDED_OR_CONTINUE( sr )

#define VLR_ON_SR_SUCCESS_RETURN_VALUE( sr ) VLR_ON_SUCCESS_RETURN_VALUE( sr )
#define VLR_ON_SR_ERROR_RETURN_VALUE( sr ) VLR_ON_ERROR_RETURN_VALUE( sr )

#define VLR_ON_HR_S_OK__RETURN_HRESULT( hr ) VLR_ON_SPECIFIC_RESULT_RETURN_VALUE( hr, S_OK )
#define VLR_ON_HR_NON_S_OK__RETURN_HRESULT( hr ) VLR_ON_NOT_SPECIFIC_RESULT_RETURN_VALUE( hr, S_OK )

#define VLR_ASSERT_ALLOCATED_OR_RETURN_STANDARD_ERROR( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, E_OUTOFMEMORY )
#define VLR_ASSERT_ALLOCATED_OR_RETURN_FAILURE_VALUE( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, _tFailureValue )

#define VLR_ASSERT_ON_UNHANDLED_SWITCH_CASE \
	VLR_ASSERTIONS_HANDLE_CHECK_FAILURE( _T("unhandled switch case") ); \
	[[fallthrough]]
