#pragma once

// Clang has a longstanding issue where a missing user-defined constructor causes default initialization to fail to compile.
// Their rationale: https://stackoverflow.com/questions/28337951/clang-compile-error-with-default-initialization
// My reaction: #boggle
#if defined(__clang__)
#define VLR_DEFINE_DEFAULT_CONSTRUCTOR_CONSTEXPR_NOEXCEPT(ClassName) constexpr ClassName() noexcept {}
#define VLR_DEFINE_DEFAULT_CONSTRUCTOR_CONSTEXPR(ClassName) constexpr ClassName() {}
#define VLR_DEFINE_DEFAULT_CONSTRUCTOR_NOEXCEPT(ClassName) ClassName() noexcept {}
#else
#define VLR_DEFINE_DEFAULT_CONSTRUCTOR_CONSTEXPR_NOEXCEPT(ClassName) constexpr ClassName() noexcept = default;
#define VLR_DEFINE_DEFAULT_CONSTRUCTOR_CONSTEXPR(ClassName) constexpr ClassName() = default;
#define VLR_DEFINE_DEFAULT_CONSTRUCTOR_NOEXCEPT(ClassName) ClassName() noexcept = default;
#endif
