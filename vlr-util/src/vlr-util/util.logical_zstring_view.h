#pragma once

#include "UtilMacros.Namespace.h"
#include "cpp_namespace.h"

#include "zstring_view.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( util )

template<class _Elem,
	class _Traits = std::char_traits<_Elem>>
	class logical_zstring_view;

// Note: Protected derivation would be better, but hard to manually expose all string_view methods

template<class _Elem, class _Traits>
class logical_zstring_view
	: public basic_zstring_view<_Elem, _Traits>
{
private:
    using this_type = logical_zstring_view<_Elem, _Traits>;
	using base_type = basic_zstring_view<_Elem, _Traits>;
    using traits_type = _Traits;
    using value_type = _Elem;
    using pointer = _Elem*;
    using const_pointer = const _Elem*;
    using reference = _Elem&;
    using const_reference = const _Elem&;
    using const_iterator = _STD _String_view_iterator<_Traits>;
    using iterator = const_iterator;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;
    using reverse_iterator = const_reverse_iterator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using backing_type_basic_string = std::basic_string<_Elem, _Traits>;

protected:
	std::optional<backing_type_basic_string> m_osValue_string;

public:
	using base_type::operator const_pointer;

public:
    decltype(auto) operator=( const base_type& svzValue )
    {
        m_osValue_string = {};
        *static_cast<base_type*>(this) = svzValue;
        return *this;
    }
    decltype(auto) operator=( const backing_type_basic_string& sValue )
    {
        m_osValue_string = sValue;
        *static_cast<base_type*>(this) = m_osValue_string.value();
        return *this;
    }
    this_type& operator=( const this_type& ) = default;

public:
	constexpr logical_zstring_view() = default;
	using base_type::base_type;
    explicit logical_zstring_view( const backing_type_basic_string& sValue )
    {
        operator=( sValue );
    }
};

using logical_azstring_view = logical_zstring_view<char>;
using logical_wzstring_view = logical_zstring_view<wchar_t>;
using logical_tzstring_view = logical_zstring_view<TCHAR>;

NAMESPACE_END //( util )

NAMESPACE_END //( vlr )
