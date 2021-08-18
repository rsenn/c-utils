# Ubuntu 12.04 LTS has CMake 2.8.7, and is an important target since several CI services, such as Travis and Drone, use it.  Solaris 11 has 2.8.6, and it's not difficult to support if you already have to support 2.8.7.
cmake_minimum_required(VERSION 2.8.6)

project(brotli C)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to Release as none was specified.")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
else()
  message(STATUS "Build type is '${CMAKE_BUILD_TYPE}'")
endif()

include(CheckCSourceCompiles)
check_c_source_compiles(
  "#if defined(__EMSCRIPTEN__)
   int main() {return 0;}
   #endif" BROTLI_EMSCRIPTEN)
if(BROTLI_EMSCRIPTEN)
  message("-- Compiler is EMSCRIPTEN")
else()
  message("-- Compiler is not EMSCRIPTEN")
endif()

get_directory_property(BROTLI_PARENT_DIRECTORY PARENT_DIRECTORY)
if(NOT DEFINED BROTLI_BUNDLED_MODE)
   if(BROTLI_PARENT_DIRECTORY)
    set(BROTLI_BUNDLED_MODE ON)
  else()
    set(BROTLI_BUNDLED_MODE OFF)
  endif()
endif()
mark_as_advanced(BROTLI_BUNDLED_MODE)

include(GNUInstallDirs)

function(hex_to_dec HEXADECIMAL DECIMAL)
  string(TOUPPER "${HEXADECIMAL}" _tail)
  set(_decimal 0)
  string(LENGTH "${_tail}" _tail_length)
  while(_tail_length GREATER 0)
    math(EXPR _decimal "${_decimal} * 16")
    string(SUBSTRING "${_tail}" 0 1 _digit)
    string(SUBSTRING "${_tail}" 1 -1 _tail)
    if(_digit STREQUAL "A")
      math(EXPR _decimal "${_decimal} + 10")
    elseif(_digit STREQUAL "B")
      math(EXPR _decimal "${_decimal} + 11")
    elseif(_digit STREQUAL "C")
      math(EXPR _decimal "${_decimal} + 12")
    elseif(_digit STREQUAL "D")
      math(EXPR _decimal "${_decimal} + 13")
    elseif(_digit STREQUAL "E")
      math(EXPR _decimal "${_decimal} + 14")
    elseif(_digit STREQUAL "F")
      math(EXPR _decimal "${_decimal} + 15")
    else()
      math(EXPR _decimal "${_decimal} + ${_digit}")
    endif()
    string(LENGTH "${_tail}" _tail_length)
  endwhile()
  set(${DECIMAL} ${_decimal} PARENT_SCOPE)
endfunction(hex_to_dec)

# Version information
file(STRINGS "c/common/version.h" _brotli_version_line REGEX "^#define BROTLI_VERSION (0x[0-9a-fA-F]+)$")
string(REGEX REPLACE "^#define BROTLI_VERSION 0x([0-9a-fA-F]+)$" "\\1" _brotli_version_hex "${_brotli_version_line}")
hex_to_dec("${_brotli_version_hex}" _brotli_version)
math(EXPR BROTLI_VERSION_MAJOR "${_brotli_version} >> 24")
math(EXPR BROTLI_VERSION_MINOR "(${_brotli_version} >> 12) & 4095")
math(EXPR BROTLI_VERSION_PATCH "${_brotli_version} & 4095")
set(BROTLI_VERSION "${BROTLI_VERSION_MAJOR}.${BROTLI_VERSION_MINOR}.${BROTLI_VERSION_PATCH}")
mark_as_advanced(BROTLI_VERSION BROTLI_VERSION_MAJOR BROTLI_VERSION_MINOR BROTLI_VERSION_PATCH)

# ABI Version information
file(STRINGS "c/common/version.h" _brotli_abi_info_line REGEX "^#define BROTLI_ABI_VERSION (0x[0-9a-fA-F]+)$")
string(REGEX REPLACE "^#define BROTLI_ABI_VERSION 0x([0-9a-fA-F]+)$" "\\1" _brotli_abi_info_hex "${_brotli_abi_info_line}")
hex_to_dec("${_brotli_abi_info_hex}" _brotli_abi_info)
math(EXPR BROTLI_ABI_CURRENT "${_brotli_abi_info} >> 24")
math(EXPR BROTLI_ABI_REVISION "(${_brotli_abi_info} >> 12) & 4095")
math(EXPR BROTLI_ABI_AGE "${_brotli_abi_info} & 4095")
math(EXPR BROTLI_ABI_COMPATIBILITY "${BROTLI_ABI_CURRENT} - ${BROTLI_ABI_AGE}")
mark_as_advanced(BROTLI_ABI_CURRENT BROTLI_ABI_REVISION BROTLI_ABI_AGE BROTLI_ABI_COMPATIBILITY)

if(ENABLE_SANITIZER)
  set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -fsanitize=${ENABLE_SANITIZER}")
  set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fsanitize=${ENABLE_SANITIZER}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${ENABLE_SANITIZER}")

  # By default, brotli depends on undefined behavior, but setting BROTLI_BUILD_PORTABLE should result in a build which does not.
  if(ENABLE_SANITIZER STREQUAL "undefined")
    add_definitions(-DBROTLI_BUILD_PORTABLE)
  endif()
endif()

include(CheckFunctionExists)
set(LIBM_LIBRARY)
check_function_exists(log2 LOG2_RES)
if(NOT LOG2_RES)
  set(orig_req_libs "${CMAKE_REQUIRED_LIBRARIES}")
  set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES};m")
  check_function_exists(log2 LOG2_LIBM_RES)
  if(LOG2_LIBM_RES)
    set(LIBM_LIBRARY "m")
    add_definitions(-DBROTLI_HAVE_LOG2=1)
  else()
    add_definitions(-DBROTLI_HAVE_LOG2=0)
  endif()

  set(CMAKE_REQUIRED_LIBRARIES "${orig_req_libs}")
  unset(LOG2_LIBM_RES)
  unset(orig_req_libs)
else()
  add_definitions(-DBROTLI_HAVE_LOG2=1)
endif()
unset(LOG2_RES)

set(BROTLI_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/c/include")
mark_as_advanced(BROTLI_INCLUDE_DIRS)

set(BROTLI_LIBRARIES_CORE brotlienc brotlidec brotlicommon)
set(BROTLI_LIBRARIES ${BROTLI_LIBRARIES_CORE} ${LIBM_LIBRARY})
mark_as_advanced(BROTLI_LIBRARIES)

set(BROTLI_LIBRARIES_CORE_STATIC brotlienc-static brotlidec-static brotlicommon-static)
set(BROTLI_LIBRARIES_STATIC ${BROTLI_LIBRARIES_CORE_STATIC} ${LIBM_LIBRARY})
mark_as_advanced(BROTLI_LIBRARIES_STATIC)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  add_definitions(-DOS_LINUX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  add_definitions(-DOS_FREEBSD)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DOS_MACOSX)
endif()

function(transform_sources_list INPUT_FILE OUTPUT_FILE)
  file(READ ${INPUT_FILE} TEXT)
  string(REGEX REPLACE "\\\\\n" "~continuation~" TEXT ${TEXT})
  string(REGEX REPLACE "([a-zA-Z_][a-zA-Z0-9_]*)[\t ]*=[\t ]*([^\n]*)" "SET(\\1 \\2)" TEXT ${TEXT})
  string(REPLACE "~continuation~" "\n" TEXT ${TEXT})
  file(WRITE ${OUTPUT_FILE} ${TEXT})
endfunction()

transform_sources_list("scripts/sources.lst" "${CMAKE_CURRENT_BINARY_DIR}/sources.lst.cmake")
include("${CMAKE_CURRENT_BINARY_DIR}/sources.lst.cmake")

if(BROTLI_EMSCRIPTEN)
  set(BROTLI_SHARED_LIBS "")
else()
  set(BROTLI_SHARED_LIBS brotlicommon brotlidec brotlienc)
  add_library(brotlicommon SHARED ${BROTLI_COMMON_C})
  add_library(brotlidec SHARED ${BROTLI_DEC_C})
  add_library(brotlienc SHARED ${BROTLI_ENC_C})
endif()

set(BROTLI_STATIC_LIBS brotlicommon-static brotlidec-static brotlienc-static)
add_library(brotlicommon-static STATIC ${BROTLI_COMMON_C})
add_library(brotlidec-static STATIC ${BROTLI_DEC_C})
add_library(brotlienc-static STATIC ${BROTLI_ENC_C})

# Older CMake versions does not understand INCLUDE_DIRECTORIES property.
include_directories(${BROTLI_INCLUDE_DIRS})

foreach(lib IN LISTS BROTLI_SHARED_LIBS)
  target_compile_definitions(${lib} PUBLIC "BROTLI_SHARED_COMPILATION")
  string(TOUPPER "${lib}" LIB)
  set_target_properties(${lib} PROPERTIES DEFINE_SYMBOL "${LIB}_SHARED_COMPILATION")
endforeach()

foreach(lib IN LISTS BROTLI_SHARED_LIBS BROTLI_STATIC_LIBS)
  target_link_libraries(${lib} ${LIBM_LIBRARY})
  set_property(TARGET ${lib} APPEND PROPERTY INCLUDE_DIRECTORIES ${BROTLI_INCLUDE_DIRS})
  set_target_properties(${lib} PROPERTIES VERSION "${BROTLI_ABI_COMPATIBILITY}.${BROTLI_ABI_AGE}.${BROTLI_ABI_REVISION}" SOVERSION "${BROTLI_ABI_COMPATIBILITY}")
  if(NOT BROTLI_EMSCRIPTEN)
    set_target_properties(${lib} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
  endif()
  set_property(TARGET ${lib} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${BROTLI_INCLUDE_DIRS}")
endforeach()

if(NOT BROTLI_EMSCRIPTEN)
  target_link_libraries(brotlidec brotlicommon)
  target_link_libraries(brotlienc brotlicommon)
endif()

target_link_libraries(brotlidec-static brotlicommon-static)
target_link_libraries(brotlienc-static brotlicommon-static)

if(BROTLI_PARENT_DIRECTORY)
  set(BROTLI_INCLUDE_DIRS "${BROTLI_INCLUDE_DIRS}" PARENT_SCOPE)
  set(BROTLI_LIBRARIES "${BROTLI_LIBRARIES}" PARENT_SCOPE)
endif()

# Installation
if(NOT BROTLI_EMSCRIPTEN)
  if(NOT BROTLI_BUNDLED_MODE)
    #install(TARGETS brotli RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

    install(TARGETS ${BROTLI_LIBRARIES_CORE} ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

    install(TARGETS ${BROTLI_LIBRARIES_CORE_STATIC} ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

    install(DIRECTORY ${BROTLI_INCLUDE_DIRS}/brotli DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
  endif() # BROTLI_BUNDLED_MODE
endif() # BROTLI_EMSCRIPTEN

# Generate a pkg-config files
function(generate_pkg_config_path outvar path)
  string(LENGTH "${path}" path_length)

  set(path_args ${ARGV})
  list(REMOVE_AT path_args 0 1)
  list(LENGTH path_args path_args_remaining)

  set("${outvar}" "${path}")

  while(path_args_remaining GREATER 1)
    list(GET path_args 0 name)
    list(GET path_args 1 value)

    get_filename_component(value_full "${value}" ABSOLUTE)
    string(LENGTH "${value}" value_length)

    if(path_length EQUAL value_length AND path STREQUAL value)
      set("${outvar}" "\${${name}}")
      break()
    elseif(path_length GREATER value_length)
      # We might be in a subdirectory of the value, but we have to be careful about a prefix matching but not being a subdirectory (for example, /usr/lib64 is not a subdirectory of /usr/lib). We'll do this by making sure the next character is a directory separator.
      string(SUBSTRING "${path}" ${value_length} 1 sep)
      if(sep STREQUAL "/")
        string(SUBSTRING "${path}" 0 ${value_length} s)
        if(s STREQUAL value)
          string(SUBSTRING "${path}" "${value_length}" -1 suffix)
          set("${outvar}" "\${${name}}${suffix}")
          break()
        endif()
      endif()
    endif()

    list(REMOVE_AT path_args 0 1)
    list(LENGTH path_args path_args_remaining)
  endwhile()

  set("${outvar}" "${${outvar}}" PARENT_SCOPE)
endfunction(generate_pkg_config_path)

function(transform_pc_file INPUT_FILE OUTPUT_FILE VERSION)
  file(READ ${INPUT_FILE} TEXT)

  set(PREFIX "${CMAKE_INSTALL_PREFIX}")
  string(REGEX REPLACE "@prefix@" "${PREFIX}" TEXT ${TEXT})
  string(REGEX REPLACE "@exec_prefix@" "${PREFIX}" TEXT ${TEXT})

  generate_pkg_config_path(LIBDIR "${CMAKE_INSTALL_FULL_LIBDIR}" prefix "${PREFIX}")
  string(REGEX REPLACE "@libdir@" "${LIBDIR}" TEXT ${TEXT})

  generate_pkg_config_path(INCLUDEDIR "${CMAKE_INSTALL_FULL_INCLUDEDIR}" prefix "${PREFIX}")
  string(REGEX REPLACE "@includedir@" "${INCLUDEDIR}" TEXT ${TEXT})

  string(REGEX REPLACE "@PACKAGE_VERSION@" "${VERSION}" TEXT ${TEXT})

  file(WRITE ${OUTPUT_FILE} ${TEXT})
endfunction()

transform_pc_file("scripts/libbrotlicommon.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/libbrotlicommon.pc" "${BROTLI_VERSION}")

transform_pc_file("scripts/libbrotlidec.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/libbrotlidec.pc" "${BROTLI_VERSION}")

transform_pc_file("scripts/libbrotlienc.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/libbrotlienc.pc" "${BROTLI_VERSION}")

if(NOT BROTLI_EMSCRIPTEN)
  if(NOT BROTLI_BUNDLED_MODE)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libbrotlicommon.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libbrotlidec.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libbrotlienc.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
  endif() # BROTLI_BUNDLED_MODE
endif() # BROTLI_EMSCRIPTEN

if(ENABLE_COVERAGE STREQUAL "yes")
  setup_target_for_coverage(coverage test coverage)
endif()
