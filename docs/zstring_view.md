### Intent

`zstring_view<charT>` is a subclass of `std::string_view<charT>`, which is intended to encapsulate an explicitly NULL-terminated string. That is, the string pointed to by the class will always have an additional NULL character (of the character type for the class) following the string view data itself. This enables the type to be used in cases where the string is required to be NULL-terminated, and the class has an implicit operator to cast to `const charT*` for these cases.

#### Suggested Naming Convention

For coding standards with type indicators in variable names, the suggested prefix for this type is 'svz' (ie: "string view zero-terminated"), to differentiate from 'sv' for a base string_view.

### Suggested Usage

`zstring_view<charT>` can be constexpr initialized at compile time, making it ideal as an alternative to having string literal variables. For example:
```c++
static constexpr auto svzFoo = zstring_view<TCHAR>{ _T( "Hello World!" ) };
```

This usage "preserves" the length of the string at compile time, with minimal overhead, allowing usage without computing the length (via strlen or equivalent) at runtime. It also inherits the advantages of using string_view when possible, as an alternative to extraneous dynamic string allocation.

The usage of `zstring_view<charT>` allows encapsulating the string literal into a class which is compatible with `std::string_view<charT>` directly (via inheritance), and/or in places where passing a string_view is more optimal than passing a pointer.

The type can also be used as an interface type for methods which would pass a string literal (expected to be NULL-terminated) to a C-style API, while preserving the ability to do efficient logging of the value within such methods (for example).

### Notes

The class has some redefinitions of some private methods from `std::string_view<charT>`, which would have been used directly if they had been protected instead, but at least in the Microsoft STL (at time of writing), they are not.

The base-class methods which are inherited return `std::string_view<charT>`, which is intended; this is a "safer" option. In particular, methods which return "slices" of the string_view may not be NULL_terminated, and thus should not return zstring_view objects.

It is possible to write more efficient conversion methods to dynamically-allocated string types also, when necessary, by utilizing the length captured at compile time. This may be an automatic gain for types which have conversions from string_view, depending on the implementation.
