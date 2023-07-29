#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "AssertionHandling_Config.h"
#include "zstring_view.h"
#include "util.IsNonZero.h"
#include "util.IsNotBlank.h"

#define VLR_ASSERTIONS_EVALUATE_COMPARISON( lhs, op, rhs ) ( (lhs) op (rhs) )

// Note: Assuming string literal at this point, so preprocessor concat works
// TODO: Convert to formatting, if/when we have the applicable library dependency

#define VLR_ASSERTIONS_HANDLE_FAILURE_NONZERO( pcszValueName ) \
	vlr::assert::HandleCheckFailure( _T("Assertion failed; nonzero value: ") pcszValueName );

#define VLR_ASSERTIONS_HANDLE_FAILURE_NONTBLANK( pcszValueName ) \
	vlr::assert::HandleCheckFailure( _T("Assertion failed; notblank value: ") pcszValueName );

#define VLR_ASSERTIONS_HANDLE_FAILURE_COMPARE( lhs, op, rhs ) \
	vlr::assert::HandleCheckFailure( _T("Assertion failed; compare failure: ") _T(#lhs) _T(#op) _T(#rhs) );

#define VLR_ASSERTIONS_RETURN_NULL return;

#define VLR_ASSERTIONS_RETURN_EXPRESSION( expression ) return (expression);

#define VLR_HANDLE_ASSERTION_FAILURE__AND_RETURN_EXPRESSION( expression ) \
{ \
	vlr::assert::HandleCheckFailure( _T("Assertion failed (general)") ); \
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

#define VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_HRESULT( hr ) { auto&& _hr = (hr); VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( SUCCEEDED(_hr), _hr ) }
#define VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_FAILURE_VALUE( hr ) { auto&& _hr = (hr); VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( SUCCEEDED(_hr), _tFailureValue ) }
#define VLR_ASSERT_HR_SUCCEEDED_OR_CONTINUE( hr ) { auto&& _hr = (hr); VLR_ASSERT_NONZERO_OR_CONTINUE( SUCCEEDED(_hr) ) }

#define VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT( sr ) { auto&& _sr = (sr); VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( _sr.isSuccess(), _sr ) }
#define VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_FAILURE_VALUE( sr ) { auto&& _sr = (sr); VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( _sr.isSuccess(), _tFailureValue ) }
#define VLR_ASSERT_SR_SUCCEEDED_OR_CONTINUE( sr ) { auto&& _sr = (sr); VLR_ASSERT_NONZERO_OR_CONTINUE( _sr.isSuccess() ) }

#define VLR_ON_HR_S_OK__RETURN_HRESULT( hr ) { auto&& _hr = (hr); if (_hr != S_OK) {} else { return _hr; } }
#define VLR_ON_HR_NON_S_OK__RETURN_HRESULT( hr ) { auto&& _hr = (hr); if (_hr == S_OK) {} else { return _hr; } }

#define VLR_ASSERT_ALLOCATED_OR_RETURN_STANDARD_ERROR( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, E_OUTOFMEMORY )
#define VLR_ASSERT_ALLOCATED_OR_RETURN_FAILURE_VALUE( value ) VLR_ASSERT_NONZERO_OR_RETURN_EXPRESSION( value, _tFailureValue )
