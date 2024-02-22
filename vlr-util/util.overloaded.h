#pragma once

namespace vlr {

namespace util {

// Note: Code suggested at cppcon talk; see also: https://en.cppreference.com/w/cpp/utility/variant/visit
// Basically, can be used to specify multiple functions for matching a variant visit method, based on type.

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace util

} // namespace vlr
