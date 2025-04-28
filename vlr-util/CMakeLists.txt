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

set(VLR_HEADER_FILES
    "AppOptionAccess.h"
    "AppOptionQualifiers.h"
    "AppOptionSource.h"
    "AppOptionSourceInfo.h"
    "AppOptionSource_File_Basic.h"
    "AppOptionSpecifiedValue.h"
    "AppOptions.h"
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
    "platform.FileSystemOps.h"
    "RegexCache.h"
    "SharedInstance.h"
    "StringCompare.h"
    "strings.split.h"
    "ThreadOperationContext.h"
    "ThreadPool.h"
    "typeshim.win32.h"
    "util.api_adaptor.OutString.h"
    "util.AutoCleanupBase.h"
    "util.choice.h"
    "util.convert.Arithmetic.h"
    "util.convert.DateTime.h"
    "util.convert.StringConversion.h"
    "util.convert.ToDisplay.h"
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
    "util.StringConversion.h"
    "UtilMacros.Assertions.h"
    "UtilMacros.General.h"
    "win32.targetver.WIN2K.h"
    "win32.targetver.WIN7.h"
    "win32.targetver.WINXP.h"
    "zstring_view.h"
)
source_group("Header Files" FILES ${VLR_HEADER_FILES})

set(VLR_SOURCE_FILES
    "AppOptionSource_File_Basic.cpp"
    "AppOptionSpecifiedValue.cpp"
    "AppOptions.cpp"
    "CloseEnough.cpp"
    "ModuleContext.Runtime.CommandLine.cpp"
    "ModuleContext.Runtime.ConfigOptionsBase.cpp"
    "ModuleContext.Runtime.cpp"
    "ops.NetworkTargetInfo.cpp"
#    "pch.cpp"
    "platform.FileSystemOps.cpp"
    "RegexCache.cpp"
    "ThreadOperationContext.cpp"
    "ThreadPool.cpp"
    "util.AutoCleanupBase.cpp"
    "util.Result.cpp"
    "util.StringConversion.cpp"
#    "vlr-util.cpp"
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

if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_GLOBAL_KEYWORD "Win32Proj"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
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
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
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
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
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

