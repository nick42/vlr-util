// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

#define VLR_CONFIG_ASSERTIONS_INCLUDE_FUNCTION
#define VLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS

#include <vlr-util/config.h>

#define GTEST_DONT_DEFINE_FAIL 1
#define GTEST_DONT_DEFINE_SUCCEED 1

#include <gtest/gtest.h>

#include <catch2/catch_all.hpp>
