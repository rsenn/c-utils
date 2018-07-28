set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(BUILD_SHARED_LIBS FALSE)

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckTypeSize)
include(FindBZip2)
include(FindPkgConfig)
include(FindZLIB)
include(UsePkgConfig)

if(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS FALSE)
else(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS TRUE)
endif(NOT ENABLE_SHARED)



set(ignored_files
  "\\\\.git.*"
  "\\\\.o"
  "\\\\.log"
  "build/.*"
  "\\\\.swp\\\\$"
  "~\\\\$"
  "fuse_hidden"
  "core\\\\.[0-9]"
  "\\\\.vscode"
  "\\\\.codelite"
  "\\\\.settings"
  "CMakeFiles\\\\/"
  "\\\\.user"
)

set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL;/")


include(CPack)

#include( "${CMAKE_CURRENT_SOURCE_DIR}/Checks.cmake" )


#if(WIN32 OR MSYS OR CYGWIN)
  add_definitions(-D_FILE_OFFSET_BITS=64)
#endif(WIN32 OR WIN64)

if(NOT MSVC)
add_definitions( -g  )
endif(NOT MSVC)


set(CMAKE_C_FLAGS_DEBUG "-Wall -DDEBUG ${CMAKE_C_FLAGS_DEBUG}")

if(NOT MSVC)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 ${CMAKE_C_FLAGS_DEBUG}")
endif(NOT MSVC)
#set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O0 -Wall")

CHECK_C_COMPILER_FLAG("-ggdb" C_COMPILER_SUPPORTS_GGDB)

CHECK_C_COMPILER_FLAG(-Wno-unused-variable, W_NO_UNUSED_VAR)
if(W_NO_UNUSED_VAR)
  add_definitions(-Wno-unused-variable)
endif(W_NO_UNUSED_VAR)
CHECK_C_COMPILER_FLAG(-Wno-unused-parameter, W_NO_UNUSED_PARAM)
if(W_NO_UNUSED_PARAM)
  add_definitions(-Wno-unused-parameter)
endif(W_NO_UNUSED_PARAM)


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


include_directories("${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

#pkg_search_module(LIBLZMA liblzma)
include(${CMAKE_SOURCE_DIR}/CheckC11Generics.cmake)
include(${CMAKE_SOURCE_DIR}/CheckSizeTypes.cmake)
include(${CMAKE_SOURCE_DIR}/CheckAlloca.cmake)
include(${CMAKE_SOURCE_DIR}/CheckDynStack.cmake)
include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)


if(LIBLZMA_FOUND)
  link_libraries(${LIBLZMA_LIBRARIES})
  include_directories(${LIBLZMA_INCLUDE_DIRS})
  add_definitions(-DHAVE_LIBLZMA=1)
endif(LIBLZMA_FOUND)

if(ZLIB_FOUND)
  link_libraries(${ZLIB_LIBRARIES})
  include_directories(${ZLIB_INCLUDE_DIRS})
  add_definitions(-DHAVE_ZLIB=1)
endif(ZLIB_FOUND)

if(BZIP2_FOUND)
  link_libraries(${BZIP2_LIBRARIES})
  include_directories(${BZIP2_INCLUDE_DIR})
endif(BZIP2_FOUND)

#pkg_search_module(LIBZ zlib)
#pkg_search_module(LIBBZ2 libbz2 bzip2)
#check_library_exists(LIBMSWSOCK ws2_32 HAVE_MSWSOCK)
check_library_exists(ws2_32 socket "" HAVE_MSWSOCK)

if(HAVE_MSWSOCK)
  set(LIBMSWSOCK ws2_32)
endif()

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



check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
#check_include_file(boost/foreach.hpp HAVE_BOOST_FOREACH_HPP)
#check_include_file(xtr1common HAVE_XTR1COMMON)
check_include_file(errno.h HAVE_ERRNO_H)
#check_include_file(initializer_list HAVE_INITIALIZER_LIST)

if(HAVE_ERRNO_H)
  add_definitions(-DHAVE_ERRNO_H=1)
endif()

check_type_size(loff_t LOFF_T)
check_type_size(lloff_t LLOFF_T)
check_type_size(off64_t OFF64_T)
check_type_size(socklen_t SOCKLEN_T)

if(NOT SOCKLEN_T)
#  add_definitions(-Dsocklen_t=int)
endif()


foreach(FUNC posix_memalign aligned_alloc _aligned_malloc)
  string(TOUPPER "HAVE_${FUNC}_FUNC" DEF)
  check_function_exists(func ${DEF})
  if(${DEF})
    add_definitions(-D${DEF}=1)
  endif()
endforeach()
check_function_exists(aligned_alloc HAVE_ALIGNED_ALLOC_FUNC)
check_function_exists(_aligned_malloc HAVE__ALIGNED_MALLOC_FUNC)

check_function_exists(stat HAVE_STAT_FUNC)
check_function_exists(lstat HAVE_LSTAT_FUNC)
check_function_exists(_llseek HAVE__LLSEEK_FUNC)
check_function_exists(_lseeki64 HAVE__LSEEKI64_FUNC)
check_function_exists(lseek HAVE_LSEEK_FUNC)
check_function_exists(llseek HAVE_LLSEEK_FUNC)
check_function_exists(lseek64 HAVE_LSEEK64_FUNC)
check_function_exists(localtime HAVE_LOCALTIME_FUNC)
check_function_exists(localtime_r HAVE_LOCALTIME_R_FUNC)
check_function_exists(setmode HAVE_SETMODE_FUNC)
check_function_exists(strncasecmp HAVE_STRNCASECMP_FUNC)
check_function_exists(strnicmp HAVE_STRNICMP_FUNC)
check_function_exists(strcasecmp HAVE_STRCASECMP_FUNC)
check_function_exists(stricmp HAVE_STRICMP_FUNC)

if(NOT HAVE_STRNCASECMP_FUNC AND HAVE_STRNICMP_FUNC)
add_definitions(-Dstrncasecmp=strnicmp)
endif(NOT HAVE_STRNCASECMP_FUNC AND HAVE_STRNICMP_FUNC)

if(NOT HAVE_STRCASECMP_FUNC AND HAVE_STRICMP_FUNC)
add_definitions(-Dstrcasecmp=stricmp)
endif(NOT HAVE_STRCASECMP_FUNC AND HAVE_STRICMP_FUNC)

if(HAVE_SETMODE_FUNC)
 add_definitions(-DHAVE_SETMODE=1)
 endif()

foreach(SEEK_FN LSEEK LLSEEK LSEEK64)
  if(HAVE_${SEEK_FN}_FUNC)
    add_definitions(-DHAVE_${SEEK_FN}=1)
  endif()
endforeach()

#check_library_exists(ws2_32, WSAStartup "" HAVE_WS2_32_LIBRARY)
if(HAVE_WS2_32_LIBRARY)
link_libraries(ws2_32)
endif(HAVE_WS2_32_LIBRARY)

if(HAVE_LSEEK64_FUNC)
  add_definitions(-DLSEEK=lseek64)
elseif(HAVE__LLSEEK_FUNC)
  add_definitions(-DLSEEK=_llseek)
elseif(HAVE__LSEEKI64_FUNC)
  add_definitions(-DLSEEK=_lseeki64)
elseif(HAVE_LLSEEK_FUNC)
  add_definitions(-DLSEEK=llseek)

elseif(HAVE_LSEEK)
  add_definitions(-DLSEEK=lseek)
endif()

check_function_exists(free HAVE_FREE_FUNC)
check_function_exists(malloc HAVE_MALLOC_FUNC)
check_function_exists(opendir HAVE_OPENDIR_FUNC)
check_function_exists(readdir HAVE_READDIR_FUNC)

check_function_exists(getdents64 HAVE_GETDENTS64_FUNC)
check_function_exists(getdents HAVE_GETDENTS_FUNC)



if(HAVE_LSTAT_FUNC)
  if(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
    set(USE_READDIR TRUE)
  else(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
    set(USE_READDIR FALSE)
  endif(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
endif(HAVE_LSTAT_FUNC)

check_function_exists(getopt_long HAVE_GETOPT_LONG_FUNC)
check_function_exists(getopt HAVE_GETOPT_FUNC)

if(NOT HAVE_GETOPT_LONG_FUNC OR NOT HAVE_GETOPT_FUNC)
  set(GETOPT_SOURCES ${CMAKE_SOURCE_DIR}/lib/getopt.c)
endif()

#check_library_exists(m, atan2 "" HAVE_MATH_LIBRARY)
check_library_exists(m roundf "" HAVE_MATH_LIBRARY)

if(HAVE_MATH_LIBRARY)
  set(MATH_LIBRARY m)
else()
  check_function_exists(atan2 HAVE_ATAN2_FUNC)
  check_function_exists(roundf HAVE_ROUNDF_FUNC)

endif(HAVE_MATH_LIBRARY)




#include_directories( "${CMAKE_SOURCE_DIR}/lib" )
include_directories( "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
include_directories( ${Iconv_INCLUDE_DIR} )

if(STATIC_LINK)
  add_definitions(-DNOLIBTOOL=1 -DLIBXML_STATIC=1)
else(STATIC_LINK)
  add_definitions(-DPIC=1)
endif(STATIC_LINK)


if(USE_READDIR)
  add_definitions(-DUSE_READDIR=1)
endif(USE_READDIR)

configure_file("${CMAKE_SOURCE_DIR}/config.h.cmake"
               "${CMAKE_BINARY_DIR}/config.h")

configure_file("${CMAKE_SOURCE_DIR}/lib/buffer.h.cmake" "${CMAKE_BINARY_DIR}/lib/buffer.h")
configure_file("${CMAKE_SOURCE_DIR}/lib/charbuf.h.cmake" "${CMAKE_BINARY_DIR}/lib/charbuf.h")


file(GLOB COMMON_C_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "*_*.c" "umult*.c" "*.h")
set(COMMON_CXX_SOURCES lib/directory_iterator.cpp lib/file.cpp lib/intelhex.cpp)

set(PROGRAMS
access compiler-wrapper count-depth decode-ls-lR eagle-init-brd hexedit httptest impgen list-r main mediathek-list mediathek-parser ntldd opensearch-dump pathtool plsconv rdir-test reg2cmd regfilter torrent-progress xmltest xmltest2 xmltest3 xmltest4
eagle-gen-cmds eagle-to-circuit buffertest
)

set(CPACK_PACKAGE_EXECUTABLES ${PROGRAMS})

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
set(LIBSUBDIRS array buffer byte case cb cbmap dir fmt hmap http iarray io list map mmap ndelay open pe playlist rdir scan socket str stralloc strarray strlist tai taia uint16 uint32 uint64 xml textbuf charbuf)

foreach(ARCHIVE ${LIBSUBDIRS})
  file(GLOB MODHEADERS lib/${ARCHIVE}.h lib/${ARCHIVE}_*.h)
  file(GLOB MODSOURCES lib/${ARCHIVE}/*.c)
  if(NOT MODSOURCES)
    file(GLOB MODSOURCES lib/${ARCHIVE}_*.c)
  endif()

  if(ARCHIVE STREQUAL io)
    list(APPEND MODSOURCES lib/iopause.c)
  endif()
  if(ARCHIVE STREQUAL str)
    list(APPEND MODSOURCES lib/time_table_spd.c lib/isleap.c)
  endif()
  if(ARCHIVE STREQUAL array)
    list(APPEND MODSOURCES lib/umult64.c)
  endif()
  if(ARCHIVE STREQUAL dir)
    list(APPEND MODSOURCES lib/fnmatch.c)
  endif()
  add_library("${ARCHIVE}" ${MODSOURCES} ${MODHEADERS})
  set_target_properties("${ARCHIVE}" PROPERTIES PREFIX "")
endforeach(ARCHIVE)

target_link_libraries(scan case)
target_link_libraries(taia tai)
target_link_libraries(io iarray)
target_link_libraries(buffer mmap stralloc str array)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  if(NOT UNIX)
    set(X64 "-x64")
  endif()
endif()

set(COMMON_LIBS strlist array buffer stralloc scan str byte mmap open fmt )

foreach(PROGRAM ${PROGRAMS})
  if("${PROGRAM}" STREQUAL ntldd)
    set(EXTRA_SOURCES libntldd.c)
  else("${PROGRAM}" STREQUAL ntldd)
    set(EXTRA_SOURCES )
  endif("${PROGRAM}" STREQUAL ntldd)

  add_executable(${PROGRAM} ${PROGRAM}.c ${EXTRA_SOURCES} ${GETOPT_SOURCES})
#  get_target_property(${PROGRAM}_INCLUDE_DIRECTORIES ${PROGRAM} INCLUDE_DIRECTORIES)
 # list(APPEND ${PROGRAM}_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

  set_target_properties(${PROGRAM} PROPERTIES
    OUTPUT_NAME "${PROGRAM}${X64}"
    INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/lib;${CMAKE_SOURCE_DIR}/lib;${LIBXML2_INCLUDE_DIR};${CMAKE_BINARY_DIR}"
    )
  target_link_libraries(${PROGRAM} ${COMMON_LIBS})
  install(TARGETS ${PROGRAM} DESTINATION bin)
endforeach(PROGRAM)

target_link_libraries(list-r dir ${COMMON_LIBS})
target_link_libraries(rdir-test rdir dir ${COMMON_LIBS})

IF(MSVC OR WIN32 OR WIN64)
  SET(SOCKET_LIB ws2_32)
endif()

list(APPEND ${LIBMSWSOCK} ${LIBLZMA_LIBRARY} ${LIBZ_LIBRARY} ${LIBBZ2_LIBRARY})

target_link_libraries(compiler-wrapper dir)
target_link_libraries(buffertest charbuf textbuf ${COMMON_LIBS} ${LIBLZMA_LIBRARIES})
target_link_libraries(eagle-init-brd xml hmap charbuf textbuf ${COMMON_LIBS} ${MATH_LIBRARY})
target_link_libraries(eagle-gen-cmds cbmap xml hmap charbuf textbuf ${COMMON_LIBS} ${MATH_LIBRARY})
target_link_libraries(eagle-to-circuit cbmap xml hmap charbuf textbuf ${COMMON_LIBS})
target_link_libraries(opensearch-dump xml hmap charbuf textbuf ${COMMON_LIBS})
target_link_libraries(plsconv playlist xml hmap buffer byte fmt scan str stralloc)
target_link_libraries(xmltest xml hmap charbuf textbuf ${COMMON_LIBS})
target_link_libraries(xmltest2 xml hmap charbuf textbuf ${COMMON_LIBS})
target_link_libraries(xmltest3 xml hmap charbuf textbuf ${COMMON_LIBS})
target_link_libraries(xmltest4 xml hmap charbuf textbuf ${COMMON_LIBS})
target_link_libraries(httptest http io socket taia tai iarray ndelay ${SOCKET_LIB} uint16 ${COMMON_LIBS})
target_link_libraries(mediathek-parser mmap open)
target_link_libraries(ntldd open)
target_link_libraries(hexedit open)
target_link_libraries(impgen open)


#target_link_libraries(eagle-init-brd)

#if(Boost_INCLUDE_DIRS)
#  include_directories("${Boost_INCLUDE_DIRS}")
#endif()

foreach(CXXPROGRAM ${CXXPROGRAMS})
#  add_executable( ${CXXPROGRAM} "${CMAKE_SOURCE_DIR}/${CXXPROGRAM}.cpp" ${COMMON_CXX_SOURCES} ${COMMON_C_SOURCES} )
  install(TARGETS ${CXXPROGRAM} DESTINATION bin)
endforeach(CXXPROGRAM)
