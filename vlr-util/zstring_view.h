#pragma once

#include <string_view>

#include "UtilMacros.Namespace.h"
#include "config.h"

#if VLR_CONFIG_INCLUDE_ATL_CString
#include <atlstr.h>
using ATL::CStringA;
using ATL::CStringW;
#endif

#ifndef __ATTR_SAL
#include "sal.h"
#endif

namespace vlr {

//#if VLR_CONFIG_INCLUDE_ATL_CString
//template<class _Elem>
//#if defined(_AFXDLL)
//using TStrTraitMFC = ATL::StrTraitMFC_DLL<_Elem>;
//#else
//using TStrTraitMFC = ATL::StrTraitMFC<_Elem>;
//#endif
//#endif

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

#if VLR_CONFIG_INCLUDE_ATL_CString
    using TCStringT = CStringT< _Elem, StrTraitATL< _Elem, ChTraitsCRT< _Elem > > >;
    using TCStringT_length = decltype(std::declval<TCStringT>().GetLength());
#endif

public:
    constexpr basic_zstring_view() noexcept = default;

    constexpr basic_zstring_view( const basic_zstring_view& ) noexcept = default;
    constexpr basic_zstring_view& operator=( const basic_zstring_view& ) noexcept = default;

    constexpr basic_zstring_view( _In_z_ const const_pointer _Ntcts ) noexcept
        : base_type{}
    {
        if (_Ntcts != nullptr)
        {
            base_type::operator=(base_type{ _Ntcts });
        }
    }

    // Note: We only allow initialization from a string pointer if you explicitly indicate that the 
    // string is null-terminated.
    struct StringIsNullTerminated {};
    explicit constexpr basic_zstring_view(
        _In_reads_( _Count ) const const_pointer _Cts,
        const size_type _Count,
        StringIsNullTerminated&& ) noexcept
        : base_type{ _Cts, _Count }
    {
		if constexpr (vlr::ModuleContext::Compilation::IsBuildType_Debug())
        {
            // Note: This is a debug-only check, since it is not guaranteed that the string is null-terminated
            // (eg: if the string is a substring of a larger string, it may not be null-terminated)
            if (_Cts != nullptr && _Cts[_Count] != _Elem{ 0 })
            {
                //vlr::assert::HandleCheckFailure(VLR_CODE_CONTEXT, VLR_ASSERTION_FUNCTION_NAME _T("Assertion failed (general)"));
				*this = {}; // Reset to empty, since we cannot guarantee that the string is null-terminated
            }
        }
    }

    // Note: Basically, we'll allow any string with the same element size to be converted to a zstring_view.
    // This allows wstring and u16string to both convert to wzstring_view, for example, IFF wchar_t is 16 bits.
    template< typename TBasicStringElem, typename std::enable_if_t<(sizeof( TBasicStringElem ) == sizeof( _Elem ))>* = nullptr >
    constexpr basic_zstring_view( const std::basic_string<TBasicStringElem>& strValue ) noexcept
        : base_type{ reinterpret_cast<const_pointer>(strValue.c_str()), strValue.length() }
    {}

	// Delete the rvalue constructor, since it would be unsafe to allow a temporary string to be used as a zstring_view 
    // (since the string would be destroyed at the end of the full expression, leaving a dangling pointer)
    template< typename TBasicStringElem, typename std::enable_if_t<(sizeof(TBasicStringElem) == sizeof(_Elem))>* = nullptr >
    constexpr basic_zstring_view(const std::basic_string<TBasicStringElem>&&) = delete;

#if VLR_CONFIG_INCLUDE_ATL_CString
    constexpr basic_zstring_view( const TCStringT& sValue ) noexcept
        : base_type{ static_cast<const const_pointer>(sValue.GetString()), static_cast<const size_type>(sValue.GetLength()) }
    {}
    constexpr basic_zstring_view(const TCStringT&&) = delete;
#endif

public:
    // Note: It is safe to return the trailing end of a null-terminated string as null-terminated
    [[nodiscard]] constexpr basic_zstring_view trailing_end( const size_type _Off = 0 ) const
    {
        _Check_offset( _Off );
        auto _Count = _Clamp_suffix_size( _Off, base_type::npos );
        return basic_zstring_view{ base_type::data() + _Off, _Count, StringIsNullTerminated{} };
    }

    [[nodiscard]] constexpr auto asStringView() const
    {
        return static_cast<const base_type&>(*this);
    }

    // It is "safe" to return the direct pointer implicitly as a null-terminated string
    // (ie: this is the anticipated main usage for this class)
    [[nodiscard]] constexpr operator const_pointer() const
    {
        return base_type::data();
    }

    [[nodiscard]] constexpr auto asConstPtr() const
    {
        return base_type::data();
    }

#if VLR_CONFIG_INCLUDE_ATL_CString
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
    //inline operator std::basic_string<_Elem>() const
    //{
    //    //return std::basic_string<_Elem>{ static_cast<const base_type&>(*this) };
    //    return toStdString();
    //}
    // Note: Not allowing for this, to eliminate ambiguity. This should also probably be explicit (see toStdString()).

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
        return (std::min)(_Size, base_type::size() - _Off);
    }

    [[noreturn]] static void _Xran() {
        throw std::out_of_range("invalid string_view position");
    }
};

// Note: This class is intended to be used for function parameters, where the parameter is expected to be a null-terminated string.
// It is different from basic_zstring_view in that it allows construction from rvalue references to std::basic_string, which is 
// not safe for basic_zstring_view (since the string would be destroyed at the end of the full expression, leaving a dangling pointer).

template<class _Elem, class _Traits = std::char_traits<_Elem>>
class basic_zstring_view_param
	: public basic_zstring_view<_Elem, _Traits>
{
private:
    using base_type = basic_zstring_view<_Elem, _Traits>;

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

#if VLR_CONFIG_INCLUDE_ATL_CString
    using typename base_type::TCStringT;
    using typename base_type::TCStringT_length;
#endif

public:
    constexpr basic_zstring_view_param() noexcept = default;

    constexpr basic_zstring_view_param( const basic_zstring_view_param& ) noexcept = default;
    constexpr basic_zstring_view_param& operator=( const basic_zstring_view_param& ) noexcept = default;

    constexpr basic_zstring_view_param( _In_z_ const const_pointer _Ntcts ) noexcept
        : base_type{}
    {
		if (_Ntcts != nullptr)
        {
            base_type::operator=(base_type{ _Ntcts });
        }
    }

    explicit constexpr basic_zstring_view_param(
        _In_reads_( _Count ) const const_pointer _Cts,
        const size_type _Count,
        typename base_type::StringIsNullTerminated&& ) noexcept
        : base_type{ _Cts, _Count, typename base_type::StringIsNullTerminated{} }
    {}

    template< typename TBasicStringElem, typename std::enable_if_t<(sizeof( TBasicStringElem ) == sizeof( _Elem ))>* = nullptr >
    constexpr basic_zstring_view_param( const std::basic_string<TBasicStringElem>& strValue ) noexcept
        : base_type{ strValue }
    {}

    // Allow construction from rvalue reference to std::basic_string (unlike base class)
    template< typename TBasicStringElem, typename std::enable_if_t<(sizeof(TBasicStringElem) == sizeof(_Elem))>* = nullptr >
    constexpr basic_zstring_view_param(const std::basic_string<TBasicStringElem>&& strValue) noexcept
        : base_type{ strValue.c_str(), strValue.length(), typename base_type::StringIsNullTerminated{} }
    {}

#if VLR_CONFIG_INCLUDE_ATL_CString
    constexpr basic_zstring_view_param( const TCStringT& sValue ) noexcept
        : base_type{ sValue }
    {}

    constexpr basic_zstring_view_param(const TCStringT&& sValue) noexcept
        : base_type{ sValue.GetString(), static_cast<size_type>(sValue.GetLength()), typename base_type::StringIsNullTerminated{} }
    {}
#endif

public:
    [[nodiscard]] constexpr basic_zstring_view_param trailing_end( const size_type _Off = 0 ) const
    {
        base_type::_Check_offset( _Off );
        auto _Count = base_type::_Clamp_suffix_size( _Off, base_type::npos );
        return basic_zstring_view_param{ base_type::data() + _Off, _Count, typename base_type::StringIsNullTerminated{} };
    }

    [[nodiscard]] constexpr auto asStringView() const
    {
        return static_cast<const std::basic_string_view<_Elem, _Traits>&>(*this);
    }

    [[nodiscard]] constexpr operator const_pointer() const
    {
        return base_type::data();
    }

    [[nodiscard]] constexpr auto asConstPtr() const
    {
        return base_type::data();
    }

#if VLR_CONFIG_INCLUDE_ATL_CString
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
        return std::basic_string<_Elem>{ static_cast<const std::basic_string_view<_Elem, _Traits>&>(*this) };
    }
};

using zstring_view = basic_zstring_view<char>;
using wzstring_view = basic_zstring_view<wchar_t>;
using tzstring_view = basic_zstring_view<TCHAR>;

using zstring_view_param = basic_zstring_view_param<char>;
using wzstring_view_param = basic_zstring_view_param<wchar_t>;
using tzstring_view_param = basic_zstring_view_param<TCHAR>;

using tstring_view = std::basic_string_view<TCHAR>;

} // namespace vlr
