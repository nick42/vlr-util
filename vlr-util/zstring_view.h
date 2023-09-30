#pragma once

#include <string_view>

#include "UtilMacros.Namespace.h"
#include "config.h"

#if VLR_CONFIG_INCLUDE_AFX
#include <atlstr.h>
#endif

#ifndef __ATTR_SAL
#include "sal.h"
#endif

VLR_NAMESPACE_BEGIN( vlr )

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
private:
    using base_type = std::basic_string_view<_Elem, _Traits>;
public:
    using typename base_type::traits_type;
    using typename base_type::value_type;
    using typename base_type::pointer;
    using typename base_type::const_pointer;
    using typename base_type::reference;
    using typename base_type::const_reference;
    using typename base_type::const_iterator;
    using typename base_type::iterator;
    using typename base_type::const_reverse_iterator;
    using typename base_type::reverse_iterator;
    using typename base_type::size_type;
    using typename base_type::difference_type;

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

    // Note: Basically, we'll allow any string with the same element size to be converted to a zstring_view.
    // This allows wstring and u16string to both convert to wzstring_view, for example, IFF wchar_t is 16 bits.
    template< typename TBasicStringElem, typename std::enable_if_t<(sizeof( TBasicStringElem ) == sizeof( _Elem ))>* = nullptr >
    constexpr basic_zstring_view( const std::basic_string<TBasicStringElem>& strValue ) noexcept
        : base_type{ reinterpret_cast<const_pointer>(strValue.c_str()), strValue.length() }
    {}

#if VLR_CONFIG_INCLUDE_AFX
    constexpr basic_zstring_view( const TCStringT& sValue ) noexcept
        : base_type{ static_cast<const_pointer>(sValue), sValue.GetLength() }
    {}
#endif

public:
    // Note: It is safe to return the trailing end of a null-terminated string as null-terminated
    [[nodiscard]] constexpr basic_zstring_view trailing_end( const size_type _Off = 0 ) const
    {
        _Check_offset( _Off );
        auto _Count = _Clamp_suffix_size( _Off, base_type::npos );
        return basic_zstring_view{ base_type::data() + _Off, _Count, StringIsNullTerminated{} };
    }

    // It is "safe" to return the direct pointer implicitly as a null-terminated string
    // (ie: this is the anticipated main usage for this class)
    [[nodiscard]] constexpr operator const_pointer() const
    {
        return base_type::data();
    }

#if VLR_CONFIG_INCLUDE_AFX
    // Allow explicit casting to CString only
    [[nodiscard]] explicit inline operator TCStringT() const
    {
        if (base_type::data() == nullptr)
        {
            return TCStringT{};
        }
        return TCStringT{ base_type::data(), static_cast<TCStringT_length>(base_type::length()) };
    }
#endif

    inline auto toStdString() const
    {
        // This just casts to base type (ie: string_view), for which there is automatic conversion to std::string
        return std::basic_string<_Elem>{ static_cast<const base_type&>(*this) };
    }

    // Allow implicit casting to std::basic_string (to sorta enumate the implicit constructor for string_view)
    inline operator std::basic_string<_Elem>() const
    {
        //return std::basic_string<_Elem>{ static_cast<const base_type&>(*this) };
        return toStdString();
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
#ifdef _WIN32
        _STD _Xout_of_range( "invalid string_view position" );
#else
        _STD __throw_out_of_range( "invalid string_view position" );
#endif
    }
};

using zstring_view = basic_zstring_view<char>;
using wzstring_view = basic_zstring_view<wchar_t>;
using tzstring_view = basic_zstring_view<TCHAR>;

using tstring_view = std::basic_string_view<TCHAR>;

VLR_NAMESPACE_END //( vlr )
