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

public:
	using base_type::base_type;
    explicit logical_zstring_view( const backing_type_basic_string& sValue )
    {
        operator=( sValue );
    }

	// Copy constructor: copy optional and re-seat base view to our buffer
	logical_zstring_view(const logical_zstring_view& other)
	{
		m_osValue_string = other.m_osValue_string;
		if (m_osValue_string.has_value())
		{
			// Re-seat base view to point to our copy of the string
			static_cast<base_type&>(*this) = base_type(m_osValue_string.value());
		}
		else
		{
			// Copy the base view from other (external view)
			static_cast<base_type&>(*this) = static_cast<const base_type&>(other);
		}
	}

	// Copy assignment: copy optional and re-seat base view to our buffer
	logical_zstring_view& operator=(const logical_zstring_view& other)
	{
		m_osValue_string = other.m_osValue_string;
		if (m_osValue_string.has_value())
		{
			// Re-seat base view to point to our copy of the string
			static_cast<base_type&>(*this) = base_type(m_osValue_string.value());
		}
		else
		{
			// Copy the base view from other (external view)
			static_cast<base_type&>(*this) = static_cast<const base_type&>(other);
		}
		return *this;
	}

	// Move constructor: move optional and re-seat base view to our buffer
	// (necessary because SSO buffers may not move, or may move to different addresses)
	logical_zstring_view(logical_zstring_view&& other) noexcept
	{
		m_osValue_string = std::move(other.m_osValue_string);
		if (m_osValue_string.has_value())
		{
			// Re-seat base view to point to our moved copy of the string
			static_cast<base_type&>(*this) = base_type(m_osValue_string.value());
		}
		else
		{
			// Move the base view from other (external view)
			static_cast<base_type&>(*this) = static_cast<base_type&&>(std::move(other));
		}
	}

	// Move assignment: move optional and re-seat base view to our buffer
	// (necessary because SSO buffers may not move, or may move to different addresses)
	logical_zstring_view& operator=(logical_zstring_view&& other) noexcept
	{
		m_osValue_string = std::move(other.m_osValue_string);
		if (m_osValue_string.has_value())
		{
			// Re-seat base view to point to our moved copy of the string
			static_cast<base_type&>(*this) = base_type(m_osValue_string.value());
		}
		else
		{
			// Move the base view from other (external view)
			static_cast<base_type&>(*this) = static_cast<base_type&&>(std::move(other));
		}
		return *this;
	}
};

using logical_azstring_view = logical_zstring_view<char>;
using logical_wzstring_view = logical_zstring_view<wchar_t>;
using logical_tzstring_view = logical_zstring_view<TCHAR>;

} // namespace util

} // namespace vlr
