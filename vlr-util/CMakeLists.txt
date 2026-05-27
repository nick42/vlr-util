cmake_minimum_required(VERSION 3.20)

project(vlr-util-cmake)

set(CMAKE_CXX_STANDARD 20)
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

file(GLOB VLR_HEADER_FILES "*.h")
list(FILTER VLR_HEADER_FILES EXCLUDE REGEX "win32\\.targetver\\.")
source_group("Header Files" FILES ${VLR_HEADER_FILES})

file(GLOB VLR_SOURCE_FILES "*.cpp")
source_group("Source Files" FILES ${VLR_SOURCE_FILES})

################################################################################
# Platform-specific files
################################################################################
if(WIN32)
    # Include legacy Windows version targeting headers
    file(GLOB WIN32_TARGETVER_HEADERS "win32.targetver.*.h")
    list(APPEND VLR_HEADER_FILES ${WIN32_TARGETVER_HEADERS})
    
    # Include Win32 subdirectory
    file(GLOB WIN32_HEADER_FILES "Win32/*.h")
    file(GLOB WIN32_SOURCE_FILES "Win32/*.cpp")
    list(APPEND VLR_HEADER_FILES ${WIN32_HEADER_FILES})
    list(APPEND VLR_SOURCE_FILES ${WIN32_SOURCE_FILES})
else()
    # Exclude Windows-only source files from non-Windows builds
    # ModuleContext.Runtime.CommandLine.cpp is entirely Windows-specific (uses GetCommandLine, CommandLineToArgvW)
    list(FILTER VLR_SOURCE_FILES EXCLUDE REGEX "ModuleContext\\.Runtime\\.CommandLine\\.cpp")
endif()

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
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /utf-8;
        /Zc:__cplusplus
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

