################################################################################
# Enforce static CRT for all MSVC configs and print flags for verification
################################################################################
if(MSVC)
    # Set static CRT for all build types
    foreach(flag_var
        CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif()
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
    # Print the flags for verification
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
    message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

set(PROJECT_NAME vlr-util.test)

# Find test dependencies
find_package(Catch2 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

################################################################################
# Source groups
################################################################################

# --- Updated file lists from .vcxproj ---
set(Header_Files
    "pch.h"
    "zstring_view.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "ActionOnDestruction.cpp"
    "AppOptionAccess.test.cpp"
    "AppOptions.test.cpp"
    "AppOptionSource_File_Basic.test.cpp"
    "AppOptionSpecifiedValue.test.cpp"
    "Assertions.test.cpp"
    "as_span.cpp"
    "AutoFreeResource.cpp"
    "AutoRevertingAssignment.cpp"
    "CaptureConsoleOutput.cpp"
    "CloseEnough.cpp"
    "enums.RangeInfo.cpp"
    "enums.SmartEnum.test.cpp"
    "formatpf.cpp"
    "logging.LogMessage.cpp"
    "logging.MessageContext.cpp"
    "pch.cpp"
    "platform.FileSystemOps.test.cpp"
    "RegexCache.cpp"
    "ResourceStringLookupValue.test.cpp"
    "SharedInstanceRegistrar.test.cpp"
    "strings.split.test.cpp"
    "StringCompare.test.cpp"
    "TestAssignment.cpp"
    "ThreadOperationContext.test.cpp"
    "ThreadPool.cpp"
    "util.api_adaptor.OutString.cpp"
    "util.choice.cpp"
    "util.convert.StringConversion.test.cpp"
    "util.data_adaptor.MultiSZ.cpp"
    "util.IsBitSet.cpp"
    "util.IsBitValue.cpp"
    "util.IsNonZero.cpp"
    "util.IsNotBlank.cpp"
    "util.logical_zstring_view.cpp"
    "util.MakeResultCode.test.cpp"
    "util.MRUCache.cpp"
    "util.NonOwningSharedPtr.cpp"
    "util.range_checked_cast.cpp"
    "util.Result.test.cpp"
    "util.StringConversion.test.cpp"
    "vlr-util.test.cpp"
    "zstring_view.cpp"
    # The following files are always excluded from build in all configs/platforms, so omitted:
    # win32.filesystem.enum_files.cpp
    # win32.filesystem.enum_volumes.cpp
    # win32.registry.RegKey.cpp
    # win32.structure.WIN32_FIND_DATA.cpp
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)

# MSVC: Whole program optimization (Release) and incremental linking (Debug/Release)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/GL>)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/INCREMENTAL>
        $<$<CONFIG:Release>:/INCREMENTAL:NO>
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

# Windows/MSVC-specific compile options and definitions
set(ROOT_NAMESPACE vlrutiltest)
if(WIN32 AND MSVC)
    # Force static CRT and enable exception handling for all build types
    foreach(flag_var
        CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif()
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
        if(NOT ${flag_var} MATCHES "/EHsc")
            set(${flag_var} "${${flag_var}} /EHsc")
        endif()
    endforeach()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        /utf-8
        /permissive-
        /std:c++17
        /W3
        /sdl
        /MP
        $<$<CONFIG:Release>:/Oi;/Gy>
        $<$<CONFIG:Debug>:/Zi>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        /DEBUG
        /SUBSYSTEM:CONSOLE
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32
        _CONSOLE
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        UNICODE
        _UNICODE
    )
    target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
    target_include_directories(${PROJECT_NAME} PRIVATE "." "..")
endif()


# Link with main library, gtest, and catch2
target_link_libraries(${PROJECT_NAME} PRIVATE vlr-util GTest::gtest GTest::gtest_main Catch2::Catch2 Catch2::Catch2WithMain)

# Enable CTest integration
enable_testing()
add_test(NAME vlr-util-test COMMAND $<TARGET_FILE:vlr-util.test>)

