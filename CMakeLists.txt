set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(BUILD_SHARED_LIBS FALSE)

include( CheckCCompilerFlag )
include( CheckCXXCompilerFlag )

include( FindLibXml2 ) # LIBXML2_FOUND LIBXML2_DEFINITIONS LIBXML2_INCLUDE_DIR LIBXML2_LIBRARIES LIBXML2_VERSION_STRING LIBXML2_XMLLINT_EXECUTABLE
include( FindZLIB )    # ZLIB_FOUND ZLIB_INCLUDE_DIR ZLIB_INCLUDE_DIRS ZLIB_LIBRARIES ZLIB_LIBRARY ZLIB_ROOT
include( FindBZip2 )   # BZIP2_FOUND BZIP2_INCLUDE_DIR BZIP2_INCLUDE_DIRS BZIP2_LIBRARIES BZIP2_LIBRARY

include( FindLibLZMA ) # LIBLZMA_FOUND LIBLZMA_INCLUDE_DIR LIBLZMA_INCLUDE_DIRS LIBLZMA_LIBRARIES LIBLZMA_LIBRARY
include( FindOpenSSL ) # OPENSSL_CRYPTO_LIBRARIES OPENSSL_CRYPTO_LIBRARY OPENSSL_SSL_LIBRARIES OPENSSL_SSL_LIBRARY LIB_EAY_LIBRARY SSL_EAY_LIBRARY 
                       # OPENSSL_INCLUDE_DIR OPENSSL_LIBRARIES 

#include( "${CMAKE_CURRENT_SOURCE_DIR}/Checks.cmake" )

add_definitions( -g )

include( FindPkgConfig )
include( UsePkgConfig )

set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -DDEBUG")
#set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O0 -Wall")

CHECK_C_COMPILER_FLAG("-ggdb" C_COMPILER_SUPPORTS_GGDB)

if(C_COMPILER_SUPPORTS_GGDB)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -ggdb")
endif(C_COMPILER_SUPPORTS_GGDB)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -DDEBUG")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O0 -Wall")

CHECK_CXX_COMPILER_FLAG("-ggdb" CXX_COMPILER_SUPPORTS_GGDB)

if(CXX_COMPILER_SUPPORTS_GGDB)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -ggdb")
endif(CXX_COMPILER_SUPPORTS_GGDB)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

include( FindBoost )
#include(CMakeDetermineCompileFeatures)
#include(CheckCXX11Features)

#pkg_search_module(LIBXML2 libxml-2.0 libxml2 libxml)
#pkg_search_module(LIBLZMA liblzma)
#pkg_search_module(LIBZ zlib)
#pkg_search_module(LIBBZ2 libbz2 bzip2)
#pkg_search_module(LIBSSL openssl libssl)

if(NOT CXX11_FLAGS)
set(CXX11_FLAGS "-std=c++11")
endif()

if(CXX11_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
#  add_definitions(-std=c++11)
endif()

if(HAS_CXX11_AUTO AND HAS_CXX11_NULLPTR AND HAS_CXX11_LAMBDA AND HAS_CXX11_STATIC_ASSERT AND HAS_CXX11_RVALUE_REFERENCES AND HAS_CXX11_DECLTYPE AND HAS_CXX11_CSTDINT_H AND HAS_CXX11_LONG_LONG)
  message(STATUS "Enabling C++11 support")
  set(CXX11 TRUE)
endif()


include( CheckIncludeFile )

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file(boost/foreach.hpp HAVE_BOOST_FOREACH_HPP)
check_include_file(xtr1common HAVE_XTR1COMMON)
check_include_file(initializer_list HAVE_INITIALIZER_LIST)

include( CheckTypeSize )

check_type_size(loff_t LOFF_T)
check_type_size(lloff_t LLOFF_T)
check_type_size(off64_t OFF64_T)

include( CheckFunctionExists )

check_function_exists(alloca HAVE_ALLOCA_FUNC)
check_function_exists(stat HAVE_STAT_FUNC)
check_function_exists(lstat HAVE_LSTAT_FUNC)
check_function_exists(lseek HAVE_LSEEK_FUNC)
check_function_exists(llseek HAVE_LLSEEK_FUNC)
check_function_exists(lseek64 HAVE_LSEEK64_FUNC)
check_function_exists(localtime HAVE_LOCALTIME_FUNC)
check_function_exists(localtime_r HAVE_LOCALTIME_R_FUNC)

check_function_exists(free HAVE_FREE_FUNC)
check_function_exists(malloc HAVE_MALLOC_FUNC)
check_function_exists(opendir HAVE_OPENDIR_FUNC)
check_function_exists(readdir HAVE_READDIR_FUNC)

if(HAVE_LSTAT_FUNC)
  if(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
    set(USE_READDIR TRUE)
  else(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
    set(USE_READDIR FALSE)
  endif(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
endif(HAVE_LSTAT_FUNC)

include( FindPkgConfig )

pkg_search_module(
				LIBXML2
				libxml-2.0 
				#REQUIRED
)


#find_package(libxml2)
include_directories( "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" ) 
include_directories( ${LIBXML2_INCLUDE_DIRS} )

#if(LIBXML2_LIBRARIES MATCHES static)
#  option(STATIC_LINK "Compile as static libraries" ON)
#else()
#  option(STATIC_LINK "Compile as static libraries" OFF)
#endif()

if(STATIC_LINK)
  add_definitions(-DNOLIBTOOL=1 -DLIBXML_STATIC=1)
else(STATIC_LINK)
  add_definitions(-DPIC=1)
endif(STATIC_LINK)


include_directories( ${LIBXML2_INCLUDE_DIR} )
set(LIBS ${LIBS} ${LIBXML2_LIBRARIES})

if(LIBXML2_INCLUDE_DIR)
  include_directories("${LIBXML2_INCLUDE_DIR}" "${LIBXML2_INCLUDE_DIR}/libxml")
endif()

if(USE_READDIR)
  add_definitions(-DUSE_READDIR=1)
endif(USE_READDIR)

configure_file("${CMAKE_SOURCE_DIR}/config.h.cmake"
               "${CMAKE_BINARY_DIR}/config.h")

file(GLOB COMMON_C_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "*_*.c" "umult*.c" "*.h")
set(COMMON_CXX_SOURCES lib/directory_iterator.cpp lib/file.cpp lib/intelhex.cpp)

set(PROGRAMS compiler-wrapper count-depth decode-ls-lR eagle-init-brd httptest list-r mediathek-list mediathek-parser opensearch-dump
  #picc-wrapper picc18-wrapper xc8-wrapper sdcc-wrapper 
  plsconv rdir-test reg2cmd regfilter torrent-progress xmltest xmltest2 xmltest3)

set(CXXPROGRAMS #kbd-adjacency
#ls-R
 #   piccfghex
 )

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

include_directories( "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/lib" "${CMAKE_BINARY_DIR}" )

add_definitions(-DHAVE_CONFIG_H)

set(HAVE_VARS HAVE_ALLOCA_H HAVE_BOOST_FOREACH_HPP HAVE_INTTYPES_H HAVE_LIBGEN_H HAVE_STDINT_H HAVE_SYS_MMAN_H HAVE_SYS_STAT_H HAVE_SYS_TYPES_H)

foreach(V ${HAVE_VARS})
  if(${V})
    add_definitions(-D${V}=1)
  endif()
endforeach()


#file(GLOB LIBOWFAT_SOURCES lib/*.c lib/*.h)
#
#add_library(libowfat ${LIBOWFAT_SOURCES})
#
#if("${CMAKE_STATIC_LIBRARY_PREFIX}" STREQUAL "lib")
#    set_target_properties(libowfat PROPERTIES
#        ARCHIVE_OUTPUT_NAME owfat
#        LIBRARY_OUTPUT_NAME owfat
#    )
#endif()


#
foreach(ARCHIVE array buffer byte dir fmt hmap http iarray io list mmap ndelay open playlist rdir scan socket str stralloc strarray strlist tai taia time uint16 uint32 uint64)
  file(GLOB MODSOURCES lib/${ARCHIVE}_*.c lib/${ARCHIVE}/*.c)

  if(ARCHIVE STREQUAL io)
    list(APPEND MODSOURCES lib/iopause.c)
  endif()
  if(ARCHIVE STREQUAL str)
    list(APPEND MODSOURCES lib/time_table_spd.c lib/isleap.c)
  endif()
  if(ARCHIVE STREQUAL array)
    list(APPEND MODSOURCES lib/umult64.c)
  endif()
  add_library("${ARCHIVE}" STATIC ${MODSOURCES})
  set_target_properties("${ARCHIVE}" PROPERTIES PREFIX "")
endforeach(ARCHIVE)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  if(NOT UNIX)
    set(X64 "-x64")
  endif()
endif()

set(COMMON_LIBS strlist array stralloc buffer open mmap fmt scan str byte)

foreach(PROGRAM ${PROGRAMS})
  add_executable(${PROGRAM} ${PROGRAM}.c)
  set_target_properties(${PROGRAM} PROPERTIES OUTPUT_NAME "${PROGRAM}${X64}")
  target_link_libraries(${PROGRAM} ${COMMON_LIBS})
  install(TARGETS ${PROGRAM} DESTINATION bin) 
endforeach(PROGRAM)

target_link_libraries(list-r dir ${COMMON_LIBS})
target_link_libraries(rdir-test rdir dir ${COMMON_LIBS})

  string(REGEX REPLACE "/lib/xml2.lib" "/bin/xml2.dll" LIBXML2_DLL "${LIBXML2_LIBRARIES}")
  
target_link_libraries(eagle-init-brd ${LIBXML2_LIBRARIES} m)
target_link_libraries(opensearch-dump ${LIBXML2_LIBRARIES})
target_link_libraries(plsconv ${LIBXML2_LIBRARIES})
target_link_libraries(xmltest ${LIBXML2_LIBRARIES})
target_link_libraries(xmltest2 ${LIBXML2_LIBRARIES})
target_link_libraries(xmltest3 ${LIBXML2_LIBRARIES})
 
  
#if(STATIC_LINK)
#  install(FILES ${LIBXML2_LIBRARIES} DESTINATION lib) 
#else()
#  install(FILES ${LIBXML2_DLL} DESTINATION bin) 
#endif()

#target_link_libraries(eagle-init-brd)

if(Boost_INCLUDE_DIRS)
  include_directories("${Boost_INCLUDE_DIRS}")
endif()

foreach(CXXPROGRAM ${CXXPROGRAMS})
#  add_executable( ${CXXPROGRAM} "${CMAKE_SOURCE_DIR}/${CXXPROGRAM}.cpp" ${COMMON_CXX_SOURCES} ${COMMON_C_SOURCES} )
  install(TARGETS ${CXXPROGRAM} DESTINATION bin) 
endforeach(CXXPROGRAM)
