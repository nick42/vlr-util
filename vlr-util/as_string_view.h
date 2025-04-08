#pragma once

#include <string_view>

#include "config.h"
#include "zstring_view.h"

namespace vlr {

class as_string_view
	: public std::string_view
{
private:
	using base_type = std::string_view;

public:
	using base_type::base_type;
	constexpr as_string_view( base_type&& value )
		: base_type{ value }
	{}
	constexpr as_string_view( const base_type& value )
		: base_type{ value }
	{}
	constexpr as_string_view( const std::string& value )
		: base_type{ vlr::zstring_view{ value }  }
	{}
#if VLR_CONFIG_INCLUDE_ATL_CString
	constexpr as_string_view( const CStringA& sValue ) noexcept
		: base_type{ static_cast<const_pointer>(sValue), sValue.GetLength() }
	{}
#endif
};

class as_wstring_view
	: public std::wstring_view
{
private:
	using base_type = std::wstring_view;

public:
	using base_type::base_type;
	constexpr as_wstring_view( base_type&& value )
		: base_type{ value }
	{}
	constexpr as_wstring_view( const base_type& value )
		: base_type{ value }
	{}
	constexpr as_wstring_view( const std::wstring& value )
		: base_type{ cpp::wzstring_view{ value } }
	{}
	constexpr as_wstring_view( const std::u16string& value )
		: base_type{ cpp::wzstring_view{ value } }
	{}
#if VLR_CONFIG_INCLUDE_ATL_CString
	constexpr as_wstring_view( const CStringW& sValue ) noexcept
		: base_type{ static_cast<const_pointer>(sValue), sValue.GetLength() }
	{}
#endif
#if VLR_CONFIG_INCLUDE_WIN32_bstr_t
	constexpr as_wstring_view( const _bstr_t& bsValue ) noexcept
		: base_type{ static_cast<const_pointer>(bsValue), bsValue.length() }
	{}
	constexpr as_wstring_view( const ATL::CComBSTR& bsValue ) noexcept
		: base_type{ static_cast<const_pointer>(bsValue), bsValue.Length() }
	{}
#endif
};

} // namespace vlr
