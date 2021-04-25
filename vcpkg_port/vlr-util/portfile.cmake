if(NOT VCPKG_TARGET_IS_WINDOWS)
    message(FATAL_ERROR "\n${PORT} does not support your system, only Windows for now.\n")
endif()

set(VERSION 0.1)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO nick42/vlr-util
#    REF bd59ab8a0f490c64ffb1a61aade5cdfb51391c79
#    SHA512 0
    HEAD_REF master
)

vcpkg_install_msbuild(
    SOURCE_PATH "${SOURCE_PATH}"
    PROJECT_SUBPATH "vlr-util/vlr-util.vcxproj"
    PLATFORM ${BUILD_ARCH}
    USE_VCPKG_INTEGRATION
    LICENSE_SUBPATH "LICENSE.txt"
    SKIP_CLEAN
)

file(COPY
    "${SOURCE_PATH}/vlr-util/"
    DESTINATION "${CURRENT_PACKAGES_DIR}/include/vlr-util/"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "pch.h" EXCLUDE
    PATTERN "targetver.h" EXCLUDE
)

#file(INSTALL ${SOURCE_PATH}/LICENSE.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/vlr-util RENAME copyright)

if(NOT DEFINED VCPKG_BUILD_TYPE OR VCPKG_BUILD_TYPE STREQUAL "release")
    set(BUILD_TREE_PATH ${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-rel)
    file(GLOB_RECURSE PDB_FILES ${BUILD_TREE_PATH}/*.pdb)
    if(PDB_FILES)
        file(COPY ${PDB_FILES} DESTINATION ${CURRENT_PACKAGES_DIR}/lib)
    endif()
endif()

if(NOT DEFINED VCPKG_BUILD_TYPE OR VCPKG_BUILD_TYPE STREQUAL "debug")
    set(BUILD_TREE_PATH ${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-dbg)
    file(GLOB_RECURSE PDB_FILES ${BUILD_TREE_PATH}/*.pdb)
    if(PDB_FILES)
        file(COPY ${PDB_FILES} DESTINATION ${CURRENT_PACKAGES_DIR}/debug/lib)
    endif()
endif()

vcpkg_clean_msbuild()
