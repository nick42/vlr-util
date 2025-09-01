# Force static CRT globally for all MSVC targets (including precompiled headers)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
endif()







cmake_minimum_required(VERSION 3.16)
project(vlr-util-cmake CXX)

# --- CRT Linkage Configuration ---
# This project is currently configured to use static CRT linkage (/MT, /MTd) for all builds.
# This is enforced by using the vcpkg static triplet (e.g., x64-windows-static).
#
# To build with static CRT:
#   - Use: -DVCPKG_TARGET_TRIPLET=x64-windows-static when running cmake and vcpkg install
#   - All vcpkg dependencies will be built with static CRT as well.
#
# To support both static and dynamic CRT linkage in the future:
#   - Add a CMake option (e.g., VLR_UTIL_USE_STATIC_CRT) and set the triplet accordingly.
#   - Consider using toolchain logic or generator expressions to propagate the correct flags.
#   - Document the supported triplets and usage in the README.

# --- vcpkg manifest mode integration ---
# If using vcpkg, pass the toolchain file to CMake:
#   -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake
# vcpkg.json should be present in the project root for manifest mode.

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific configuration
if(MSVC)
    # Remove default MSVC flags if needed
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
else()
    # GCC/Clang: add common warning flags
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Common utils
include(CMake/Utils.cmake)
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

# Use solution folders feature if supported
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Sub-projects
add_subdirectory(vlr-util)
add_subdirectory(vlr-util.test)
