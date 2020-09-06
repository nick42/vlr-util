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

#define ASSERT_NONZERO__OR_RETURN( value ) \
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

#define ASSERT_NOTBLANK__OR_RETURN( value ) \
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

#define ASSERT_COMPARE__OR_RETURN( lhs, op, rhs ) \
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

#define ASSERT_NONZERO__OR_RETURN_EXPRESSION( value, expression ) \
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

#define ASSERT_NOTBLANK__OR_RETURN_EXPRESSION( value, expression ) \
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

#define ASSERT_COMPARE__OR_RETURN_EXPRESSION( lhs, op, rhs, expression ) \
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

#define ASSERT_HR_SUCCEEDED__OR_RETURN_HRESULT( hr ) { auto&& _hr = (hr); ASSERT_NONZERO__OR_RETURN_EXPRESSION( SUCCEEDED(_hr), _hr ) }

#define ON_HR_NON_S_OK__RETURN_HRESULT( hr ) { auto&& _hr = (hr); if (_hr == S_OK) {} else { return _hr; } }
