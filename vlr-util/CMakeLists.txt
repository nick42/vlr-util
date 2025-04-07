cmake_minimum_required(VERSION 3.14)

project(vlr-util)

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

set(VLR_HEADER_FILES
    "ActionOnDestruction.h"
    "as_span.h"
    "as_string_view.h"
    "AssertionHandling_Config.h"
    "AutoFreeResource.h"
    "AutoRevertingAssignment.h"
    "BaseWithVirtualDestructor.h"
    "c_runtime.StringFunctions.h"
    "CloseEnough.h"
    "config.h"
    "cpp_namespace.h"
    "enums.FormatEnum.h"
    "enums.RangeInfo.h"
    "formatpf.h"
    "framework.h"
    "include.fmt.h"
    "include.spdlog.h"
    "logging.LogMessage.h"
    "logging.MessageContext.h"
    "ModuleContext.Compilation.h"
    "ModuleContext.Runtime.CommandLine.h"
    "ModuleContext.Runtime.ConfigOptionsBase.h"
    "ModuleContext.Runtime.h"
    "ops.NetworkTargetInfo.h"
    "pch.h"
    "RegexCache.h"
    "SharedInstance.h"
    "StringCompare.h"
    "strings.split.h"
    "ThreadPool.h"
    "typeshim.win32.ASSERT.h"
    "typeshim.win32.h"
    "util.api_adaptor.OutString.h"
    "util.AutoCleanup_SC_HANDLE.h"
    "util.AutoCleanupBase.h"
    "util.choice.h"
    "util.convert.Arithmetic.h"
    "util.convert.DateTime.h"
    "util.convert.StringConversion.h"
    "util.convert.ToDisplay.h"
    "util.CStringBufferAccess.h"
    "util.data_adaptor.MultiSZ.h"
    "util.includes.h"
    "util.IsBitSet.h"
    "util.IsBitValue.h"
    "util.IsNonZero.h"
    "util.IsNotBlank.h"
    "util.LevenshteinDistance.h"
    "util.logical_zstring_view.h"
    "util.MathOps.h"
    "util.NonOwningSharedPtr.h"
    "util.OnValidAssignTo.h"
    "util.range_checked_cast.h"
    "util.Result.h"
    "util.static_assert.h"
    "util.std_aliases.h"
    "util.types.h"
    "util.types.linux.h"
    "util.types.win32.h"
    "util.Unicode.h"
    "UtilMacros.Assertions.h"
    "UtilMacros.General.h"
    "UtilMacros.Namespace.h"
    "win32.targetver.WIN2K.h"
    "win32.targetver.WIN7.h"
    "win32.targetver.WINXP.h"
    "zstring_view.h"
)
source_group("Header Files" FILES ${VLR_HEADER_FILES})

set(VLR_SOURCE_FILES
    "CloseEnough.cpp"
    "ModuleContext.Runtime.CommandLine.cpp"
    "ModuleContext.Runtime.ConfigOptionsBase.cpp"
    "ModuleContext.Runtime.cpp"
    "ops.NetworkTargetInfo.cpp"
    "pch.cpp"
    "RegexCache.cpp"
    "ThreadPool.cpp"
    "util.AutoCleanup_SC_HANDLE.cpp"
    "util.AutoCleanupBase.cpp"
    "util.Result.cpp"
    "util.Unicode.cpp"
    "vlr-util.cpp"
)
source_group("Source Files" FILES ${VLR_SOURCE_FILES})

set(ALL_FILES
    ${no_group_source_files}
    ${VLR_HEADER_FILES}
    ${VLR_SOURCE_FILES}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC 
	${ALL_FILES}
	fmt::fmt-header-only
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)

# use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Includes for CMake from *.props
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    use_props(${PROJECT_NAME} Debug   "Project.cmake")
    use_props(${PROJECT_NAME} Release "Project.cmake")
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    use_props(${PROJECT_NAME} Debug   "Project.cmake")
    use_props(${PROJECT_NAME} Release "Project.cmake")
endif()

set(ROOT_NAMESPACE vlrutil)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_LIB;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /MP;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /MP;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
        )
    endif()
endif()

