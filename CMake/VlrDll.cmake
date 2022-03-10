include(CMakeParseArguments)
include(GNUInstallDirs)

set(VLR_INTERNAL_DLL_FILES
)

function(vlr_internal_dll_contains)
  cmake_parse_arguments(VLR_INTERNAL_DLL
    ""
    "OUTPUT;TARGET"
    ""
    ${ARGN}
  )

  STRING(REGEX REPLACE "^vlr::" "" _target ${VLR_INTERNAL_DLL_TARGET})

  list(FIND
    VLR_INTERNAL_DLL_TARGETS
    "${_target}"
    _index)

  if (${_index} GREATER -1)
    set(${VLR_INTERNAL_DLL_OUTPUT} 1 PARENT_SCOPE)
  else()
    set(${VLR_INTERNAL_DLL_OUTPUT} 0 PARENT_SCOPE)
  endif()
endfunction()

function(vlr_internal_dll_targets)
  cmake_parse_arguments(VLR_INTERNAL_DLL
  ""
  "OUTPUT"
  "DEPS"
  ${ARGN}
  )

  set(_deps "")
  foreach(dep IN LISTS VLR_INTERNAL_DLL_DEPS)
    vlr_internal_dll_contains(TARGET ${dep} OUTPUT _contains)
    if (_contains)
      list(APPEND _deps vlr_dll)
    else()
      list(APPEND _deps ${dep})
    endif()
  endforeach()

  # Because we may have added the DLL multiple times
  list(REMOVE_DUPLICATES _deps)
  set(${VLR_INTERNAL_DLL_OUTPUT} "${_deps}" PARENT_SCOPE)
endfunction()

function(vlr_make_dll)
  add_library(
    vlr_dll
    SHARED
      "${VLR_INTERNAL_DLL_FILES}"
  )
  target_link_libraries(
    vlr_dll
    PRIVATE
      ${VLR_DEFAULT_LINKOPTS}
  )
  set_property(TARGET vlr_dll PROPERTY LINKER_LANGUAGE "CXX")
  target_include_directories(
    vlr_dll
    PUBLIC
      "$<BUILD_INTERFACE:${VLR_COMMON_INCLUDE_DIRS}>"
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  target_compile_options(
    vlr_dll
    PRIVATE
      ${VLR_DEFAULT_COPTS}
  )

  target_compile_definitions(
    vlr_dll
    PRIVATE
      VLR_BUILD_DLL
      NOMINMAX
    INTERFACE
      ${VLR_CC_LIB_DEFINES}
  )
  install(TARGETS vlr_dll EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endfunction()
