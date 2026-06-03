#pragma once

#include "cpp_namespace.h"

#include "zstring_view.h"

namespace vlr {

namespace util {

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

private:
	// Helper to re-seat base_type view onto m_osValue_string if it's engaged
	inline void _reseat_base_view()
	{
		if (m_osValue_string.has_value())
		{
			*static_cast<base_type*>(this) = m_osValue_string.value();
		}
		else
		{
			*static_cast<base_type*>(this) = base_type{};
		}
	}

public:
	using base_type::operator const_pointer;

public:
	inline auto& operator=(const base_type& svzValue)
	{
		m_osValue_string = {};
		*static_cast<base_type*>(this) = svzValue;
		return *this;
	}
	inline auto& operator=(const backing_type_basic_string& sValue)
	{
		m_osValue_string = sValue;
		_reseat_base_view();
		return *this;
	}

	// User-defined copy constructor: copy optional and re-seat base view
	inline logical_zstring_view(const this_type& other) noexcept
		: base_type{}
		, m_osValue_string{ other.m_osValue_string }
	{
		_reseat_base_view();
	}

	// User-defined copy assignment: copy optional and re-seat base view
	inline this_type& operator=(const this_type& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		m_osValue_string = other.m_osValue_string;
		_reseat_base_view();
		return *this;
	}

	// User-defined move constructor: move optional and re-seat base view
	inline logical_zstring_view(this_type&& other) noexcept
		: base_type{}
		, m_osValue_string{ std::move(other.m_osValue_string) }
	{
		_reseat_base_view();
		// Clear source to avoid use-after-free if source is later used
		other.m_osValue_string = {};
		*static_cast<base_type*>(&other) = base_type{};
	}

	// User-defined move assignment: move optional and re-seat base view
	inline this_type& operator=(this_type&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		m_osValue_string = std::move(other.m_osValue_string);
		_reseat_base_view();
		// Clear source to avoid use-after-free if source is later used
		other.m_osValue_string = {};
		*static_cast<base_type*>(&other) = base_type{};
		return *this;
	}

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

} // namespace util

} // namespace vlr
