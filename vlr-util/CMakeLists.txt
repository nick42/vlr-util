cmake_minimum_required(VERSION 3.20)

project(vlr-util-cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt CONFIG REQUIRED)

set(PROJECT_NAME vlr-util)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../README.md"
    "vcpkg.json"
)
source_group("" FILES ${no_group_source_files})


# --- Updated file lists from .vcxproj ---
set(VLR_HEADER_FILES
    "ActionOnDestruction.h"
    "AppOptionAccess.h"
    "AppOptionQualifiers.h"
    "AppOptions.h"
    "AppOptionSource.h"
    "AppOptionSourceInfo.h"
    "AppOptionSource_File_Basic.h"
    "AppOptionSpecifiedValue.h"
    "AssertionHandling_Config.h"
    "as_span.h"
    "as_string_view.h"
    "AutoFreeResource.h"
    "AutoRevertingAssignment.h"
    "BaseWithVirtualDestructor.h"
    "CaptureConsoleOut.h"
    "CloseEnough.h"
    "config.h"
    "cpp_namespace.h"
    "c_runtime.StringFunctions.h"
    "debugbreak.h"
    "DynamicLoadedFunction.h"
    "enums.FormatEnum.h"
    "enums.RangeInfo.h"
    "enums.SmartEnum.h"
    "formatpf.h"
    "framework.h"
    "include.fmt.h"
    "include.spdlog.h"
    "logging.Callbacks.h"
    "logging.LogMessage.DefaultImpl.h"
    "logging.LogMessage.h"
    "logging.MessageContext.h"
    "ModuleContext.Compilation.h"
    "ModuleContext.LogicalState.h"
    "ModuleContext.Runtime.CommandLine.h"
    "ModuleContext.Runtime.ConfigOptionsBase.h"
    "ModuleContext.Runtime.h"
    "ops.NetworkTargetInfo.h"
    "pch.h"
    "platform.FileSystemOps.h"
    "RegexCache.h"
    "ResourceStringLookupValue.h"
    "RetrySettings.h"
    "SharedInstance.h"
    "SharedInstanceRegistrar.h"
    "StringCompare.h"
    "strings.split.h"
    "ThreadOperationContext.h"
    "ThreadPool.h"
    "typeshim.win32.ASSERT.h"
    "typeshim.win32.h"
    "util.api_adaptor.OutString.h"
    "util.AutoCleanupBase.h"
    "util.CallWithAutoRetry.h"
    "util.choice.h"
    "util.convert.Arithmetic.h"
    "util.convert.DateTime.h"
    "util.convert.StringConversion.h"
    "util.convert.ToDisplay.h"
    "util.crc32.h"
    "util.data_adaptor.MultiSZ.h"
    "util.includes.h"
    "util.IsBitSet.h"
    "util.IsBitValue.h"
    "util.IsNonZero.h"
    "util.IsNotBlank.h"
    "util.LevenshteinDistance.h"
    "util.logical_zstring_view.h"
    "util.MakeResultCode.h"
    "util.MathOps.h"
    "util.MRUCache.h"
    "util.NonOwningSharedPtr.h"
    "util.OnValidAssignTo.h"
    "util.overloaded.h"
    "util.range_checked_cast.h"
    "util.Result.h"
    "util.static_assert.h"
    "util.std_aliases.h"
    "util.StringConversion.base.h"
    "util.types.h"
    "util.types.linux.h"
    "util.types.win32.h"
    "util.StringConversion.h"
    "UtilMacros.Assertions.h"
    "UtilMacros.CompilerSupport.h"
    "UtilMacros.General.h"
    "UtilMacros.Namespace.h"
    "win32.targetver.WIN2K.h"
    "win32.targetver.WIN7.h"
    "win32.targetver.WINXP.h"
    $<$<PLATFORM_ID:Windows>:Win32/util.CStringBufferAccess.h>
    $<$<PLATFORM_ID:Windows>:Win32/util.win32.StringConversion.h>
    "zstring_view.h"
)
source_group("Header Files" FILES ${VLR_HEADER_FILES})

set(VLR_SOURCE_FILES
    "AppOptions.cpp"
    "AppOptionSource_File_Basic.cpp"
    "AppOptionSpecifiedValue.cpp"
    "AssertionHandling_Config.cpp"
    "CaptureConsoleOut.cpp"
    "CloseEnough.cpp"
    "logging.Callbacks.cpp"
    "ModuleContext.LogicalState.cpp"
    "ModuleContext.Runtime.CommandLine.cpp"
    "ModuleContext.Runtime.ConfigOptionsBase.cpp"
    "ModuleContext.Runtime.cpp"
    "ops.NetworkTargetInfo.cpp"
    "pch.cpp"
    "platform.FileSystemOps.cpp"
    "RegexCache.cpp"
    "SharedInstanceRegistrar.cpp"
    "ThreadOperationContext.cpp"
    "ThreadPool.cpp"
    "util.AutoCleanupBase.cpp"
    "util.Result.cpp"
    "util.StringConversion.cpp"
    $<$<PLATFORM_ID:Windows>:Win32/util.win32.StringConversion.cpp>
)
source_group("Source Files" FILES ${VLR_SOURCE_FILES})

set(ALL_FILES
    ${no_group_source_files}
    ${VLR_HEADER_FILES}
    ${VLR_SOURCE_FILES}
)

option(FEATURE_ENABLE_CUSTOM_STRING_CONVERSION "Enable custom string conversions" OFF)
if(FEATURE_ENABLE_CUSTOM_STRING_CONVERSION)
	add_definitions(-DVLR_CONFIG_ENABLE_CUSTOM_STRING_CONVERSIONS)
endif()

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC 
	${ALL_FILES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	fmt::fmt
)

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

# use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
#if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
#    use_props(${PROJECT_NAME} Debug   "Project.cmake")
#    use_props(${PROJECT_NAME} Release "Project.cmake")
#elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
#    use_props(${PROJECT_NAME} Debug   "Project.cmake")
#    use_props(${PROJECT_NAME} Release "Project.cmake")
#endif()

set(ROOT_NAMESPACE vlrutil)


if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
else()
    # GCC/Clang: enable LTO in Release
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3 -flto>)
    target_link_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-flto>)
endif()

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_LIB;"
)


# Platform-specific compile definitions
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "UNICODE;"
        "_UNICODE"
    )
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_definitions(${PROJECT_NAME} PRIVATE
            "WIN32;"
        )
    endif()
else()
    # No special defines for non-MSVC
endif()

################################################################################
# Compile and link options
################################################################################


# Platform-specific compile/link options
if(MSVC)
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
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/Oi;/Gy>
        /MP
        /sdl
        /W4
        /utf-8
        /permissive-
        /std:c++17
        $<$<CONFIG:Debug>:/Zi>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/OPT:REF;/OPT:ICF>
    )
    # Whole program optimization (Release) and incremental linking (Debug/Release)
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:/GL>)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/INCREMENTAL>
        $<$<CONFIG:Release>:/INCREMENTAL:NO>
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Windows/MSVC-specific compile options and definitions
if(WIN32 AND MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        /utf-8
        /permissive-
        /std:c++17
        /W4
        /sdl
        /MP
        $<$<CONFIG:Release>:/Oi;/Gy>
        $<$<CONFIG:Debug>:/Zi>
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/OPT:REF;/OPT:ICF>
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        WIN32
        _LIB
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
        UNICODE
        _UNICODE
    )
    target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
    target_include_directories(${PROJECT_NAME} PRIVATE ".")
endif()

install(TARGETS vlr-util
        EXPORT vlr-util-targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Generate and install *-targets.cmake 
install(EXPORT vlr-util-targets
        FILE vlr-util-targets.cmake
        NAMESPACE vlr-util::
        DESTINATION share/vlr-util)

# Generate the config file in the current binary dir (this ensures it's not placed directly in source)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/vlr-util-config.cmake"
"include(CMakeFindDependencyMacro)\n"
"find_package(fmt CONFIG REQUIRED)\n"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/vlr-util-targets.cmake\")\n"
)

# Install the generated config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vlr-util-config.cmake"
        DESTINATION share/vlr-util)

