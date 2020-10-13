#pragma once

#include <string_view>

#include "UtilMacros.Namespace.h"
#include "config.h"

#if VLR_CONFIG_INCLUDE_AFX
#include <atlstr.h>
#endif

NAMESPACE_BEGIN( vlr )

#if VLR_CONFIG_INCLUDE_AFX
template<class _Elem>
#if defined(_AFXDLL)
using TStrTraitMFC = StrTraitMFC_DLL<_Elem>;
#else
using TStrTraitMFC = StrTraitMFC<_Elem>;
#endif
#endif

// Forward-declaring the class to specify the template param default (mirrors what string_view does)

template<class _Elem,
	class _Traits = std::char_traits<_Elem>>
	class basic_zstring_view;

template<class _Elem, class _Traits>
class basic_zstring_view
	: public std::basic_string_view<_Elem, _Traits>
{
public:
    using base_type = std::basic_string_view<_Elem, _Traits>;
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

#if VLR_CONFIG_INCLUDE_AFX
    using TCStringT = ATL::CStringT<_Elem, TStrTraitMFC<_Elem>>;
    using TCStringT_length = decltype(std::declval<TCStringT>().GetLength());
#endif

public:
    constexpr basic_zstring_view() noexcept = default;

    constexpr basic_zstring_view( const basic_zstring_view& ) noexcept = default;
    constexpr basic_zstring_view& operator=( const basic_zstring_view& ) noexcept = default;

    constexpr basic_zstring_view( _In_z_ const const_pointer _Ntcts ) noexcept
        : base_type{ _Ntcts }
    {}
    // Note: We only allow initialization from a string pointer if you explicitly indicate that the 
    // string is null-terminated.
    struct StringIsNullTerminated {};
    explicit constexpr basic_zstring_view(
        _In_reads_( _Count ) const const_pointer _Cts,
        const size_type _Count,
        StringIsNullTerminated&& ) noexcept
        : base_type{ _Cts, _Count }
    {}

    constexpr basic_zstring_view( const std::basic_string<_Elem>& strValue ) noexcept
        : base_type{ static_cast<const_pointer>(strValue.c_str()), strValue.length() }
    {}

#if VLR_CONFIG_INCLUDE_AFX
    constexpr basic_zstring_view( const TCStringT& sValue ) noexcept
        : base_type{ static_cast<const_pointer>(sValue), sValue.GetLength() }
    {}
#endif

public:
    // Note: It is safe to return the trailing end of a null-terminated string as null-terminated
    _NODISCARD constexpr base_type trailing_end( const size_type _Off = 0 )
    {
        _Check_offset( _Off );
        auto _Count = _Clamp_suffix_size( _Off, base_type::npos );
        return basic_zstring_view{ base_type::data() + _Off, _Count, StringIsNullTerminated{} };
    }

    // It is "safe" to return the direct pointer implicitly as a null-terminated string
    // (ie: this is the anticipated main usage for this class)
    _NODISCARD constexpr operator const_pointer() const
    {
        return base_type::data();
    }

#if VLR_CONFIG_INCLUDE_AFX
    // Allow explicit casting to CString only
    _NODISCARD explicit inline operator TCStringT() const
    {
        if (base_type::data() == nullptr)
        {
            return TCStringT{};
        }
        return TCStringT{ base_type::data(), static_cast<TCStringT_length>(base_type::length()) };
    }
#endif

    // Allow implicit casting to std::basic_string (to sorta enumate the implicit constructor for string_view)
    inline operator std::basic_string<_Elem>() const
    {
        return std::basic_string<_Elem>{ static_cast<const base_type&>(*this) };
    }

    // Note: These methods copied from basic_string_view, cause they are not protected like they probably should be
protected:
    constexpr void _Check_offset( const size_type _Off ) const { // checks whether _Off is in the bounds of [0, size()]
        if (base_type::size() < _Off) {
            _Xran();
        }
    }

    constexpr void _Check_offset_exclusive( const size_type _Off ) const {
        // checks whether _Off is in the bounds of [0, size())
        if (base_type::size() <= _Off) {
            _Xran();
        }
    }

    constexpr size_type _Clamp_suffix_size( const size_type _Off, const size_type _Size ) const noexcept {
        // trims _Size to the longest it can be assuming a string at/after _Off
        return (_STD min)(_Size, base_type::size() - _Off);
    }

    [[noreturn]] static void _Xran() {
        _STD _Xout_of_range( "invalid string_view position" );
    }
};

using zstring_view = basic_zstring_view<char>;
using wzstring_view = basic_zstring_view<wchar_t>;
using tzstring_view = basic_zstring_view<TCHAR>;

using tstring_view = std::basic_string_view<TCHAR>;

NAMESPACE_END //( vlr )
