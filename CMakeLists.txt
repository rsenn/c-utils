set(CMAKE_LEGACY_CYGWIN_WIN32 0) 
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

include(FindBoost)
#include(CMakeDetermineCompileFeatures)
include(CheckCXX11Features)

if(CXX11_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
#  add_definitions(-std=c++11)
endif()

if(HAS_CXX11_AUTO AND HAS_CXX11_NULLPTR AND HAS_CXX11_LAMBDA AND HAS_CXX11_STATIC_ASSERT AND HAS_CXX11_RVALUE_REFERENCES AND HAS_CXX11_DECLTYPE AND HAS_CXX11_CSTDINT_H AND HAS_CXX11_LONG_LONG)
  message(STATUS "Enabling C++11 support")
  set(CXX11 TRUE)
endif()


include(CheckIncludeFile)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file(boost/foreach.hpp HAVE_BOOST_FOREACH_HPP)
check_include_file(xtr1common HAVE_XTR1COMMON)
check_include_file(initializer_list HAVE_INITIALIZER_LIST)

include(CheckTypeSize)

check_type_size(loff_t LOFF_T)
check_type_size(lloff_t LLOFF_T)
check_type_size(off64_t OFF64_T)

include(CheckFunctionExists)

check_function_exists(alloca HAVE_ALLOCA_FUNC)
check_function_exists(stat HAVE_STAT_FUNC)
check_function_exists(lstat HAVE_LSTAT_FUNC)
check_function_exists(lseek HAVE_LSEEK_FUNC)
check_function_exists(llseek HAVE_LLSEEK_FUNC)
check_function_exists(lseek64 HAVE_LSEEK64_FUNC)
check_function_exists(localtime HAVE_LOCALTIME_FUNC)
check_function_exists(localtime_r HAVE_LOCALTIME_R_FUNC)

configure_file("${CMAKE_SOURCE_DIR}/config.h.cmake"
               "${CMAKE_BINARY_DIR}/config.h")

file(GLOB COMMON_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "*_*.c" "umult*.c" "*.h")

set(PROGRAMS list-r count-depth decode-ls-lR torrent-progress reg2cmd)
#set(CXXPROGRAMS kbd-adjacency)

if(MSVC)
#  add_definitions(-Dinline=__force_inline)
  add_definitions(/DINLINE=__inline)
  add_definitions(/wo4005)
  add_definitions(/W0)
else()
  add_definitions(-DINLINE=inline)
endif()

if(CXX11)
#  add_definitions(-DCXX11=1)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS} -DCXX11=1")
endif()

string(REGEX REPLACE "/W[0-9]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories("${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_definitions(-DHAVE_CONFIG_H)

set(HAVE_VARS HAVE_ALLOCA_H HAVE_BOOST_FOREACH_HPP HAVE_INTTYPES_H HAVE_LIBGEN_H HAVE_STDINT_H HAVE_SYS_MMAN_H HAVE_SYS_STAT_H HAVE_SYS_TYPES_H)

foreach(V ${HAVE_VARS})
  if(${V})
    add_definitions(-D${V}=1)
  endif()
endforeach()

foreach(PROGRAM ${PROGRAMS})
  add_executable(${PROGRAM} "${CMAKE_SOURCE_DIR}/${PROGRAM}.c" ${COMMON_SOURCES})
  install(TARGETS ${PROGRAM} DESTINATION bin) 
endforeach(PROGRAM)

if(Boost_INCLUDE_DIRS)
  include_directories("${Boost_INCLUDE_DIRS}")
endif()

foreach(CXXPROGRAM ${CXXPROGRAMS})
  add_executable(${CXXPROGRAM} "${CMAKE_SOURCE_DIR}/${CXXPROGRAM}.cpp" ${COMMON_SOURCES})
  install(TARGETS ${CXXPROGRAM} DESTINATION bin) 
endforeach(CXXPROGRAM)

