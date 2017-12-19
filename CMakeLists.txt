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

set(PROGRAMS eagle-init-brd count-depth decode-ls-lR httptest list-r mediathek-list mediathek-parser rdir-test reg2cmd regfilter torrent-progress)

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
#foreach(ARCHIVE http io mmap rdir dir socket strlist stralloc strarray hmap iarray ndelay array buffer open str fmt scan taia tai byte uint16 uint32)
#  file(GLOB MODSOURCES lib/${ARCHIVE}_*.c)
#
#  if(ARCHIVE STREQUAL io)
#    list(APPEND MODSOURCES lib/iopause.c)
#  endif()
#  if(ARCHIVE STREQUAL str)
#    list(APPEND MODSOURCES lib/time_table_spd.c lib/isleap.c)
#  endif()
#  if(ARCHIVE STREQUAL array)
#    list(APPEND MODSOURCES lib/umult64.c)
#  endif()
#  add_library("${ARCHIVE}" STATIC ${MODSOURCES})
#  set_target_properties("${ARCHIVE}" PROPERTIES PREFIX "")
#endforeach(ARCHIVE)


add_executable( compiler-wrapper compiler-wrapper.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_rchr.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_long.c lib/fmt_ulong.c lib/stralloc_append.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushm_internal.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/str_basename.c lib/str_diff.c lib/str_diffn.c lib/str_dup.c lib/str_len.c lib/str_rchr.c )
add_executable( count-depth count-depth.c lib/buffer_0.c lib/buffer_1.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_get_token.c lib/buffer_put.c lib/buffer_putulong.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/fmt_ulong.c )
add_executable( decode-ls-lR decode-ls-lR.c lib/buffer_0.c lib/buffer_1.c lib/buffer_2.c lib/buffer_close.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_get_token.c lib/buffer_put.c lib/buffer_puts.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/stralloc_catb.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/str_diffn.c lib/str_len.c )
add_executable( eagle-init-brd eagle-init-brd.c lib/byte_copy.c lib/hmap_add.c lib/hmap_destroy.c lib/hmap_free_data.c lib/hmap_init.c lib/hmap_search.c lib/hmap_truncate.c lib/scan_double.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_nul.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/str_diff.c lib/str_len.c )
add_executable( httptest httptest.c lib/buffer_1.c lib/buffer_2.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_get.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_get_token.c lib/buffer_init.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_putulong.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/byte_zero.c lib/fmt_long.c lib/fmt_ulong.c lib/http_get.c lib/http_init.c lib/http_readable.c lib/http_sendreq.c lib/iarray_allocate.c lib/iarray_get.c lib/iarray_init.c lib/iopause.c lib/io_fd.c lib/io_nonblock.c lib/ndelay_on.c lib/scan_fromhex.c lib/scan_ulong.c lib/scan_ulongn.c lib/scan_xlong.c lib/socket_connect4.c lib/socket_tcp4.c lib/socket_tcp4b.c lib/stralloc_append.c lib/stralloc_catb.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/str_chr.c lib/str_len.c lib/taia_approx.c lib/taia_frac.c lib/taia_less.c lib/taia_sub.c lib/taia_uint.c lib/uint16_pack_big.c )
add_executable( list-r list-r.c lib/array_allocate.c lib/array_catb.c lib/array_fail.c lib/array_length.c lib/array_start.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putc.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_zero.c lib/dir_close.c lib/dir_name.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_uint64.c lib/fnmatch.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_nul.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/str_diffn.c lib/str_len.c lib/umult64.c lib/utf8.c )
add_executable( mediathek-list mediathek-list.c lib/buffer_1.c lib/buffer_2.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_get.c lib/buffer_getc.c lib/buffer_get_token.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putm_internal.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putulong.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/byte_zero.c lib/fmt_ulong.c lib/fmt_ulonglong.c lib/isleap.c lib/scan_uint.c lib/scan_ushort.c lib/stralloc_append.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/strlist_at.c lib/strlist_count.c lib/strlist_dump.c lib/strlist_join.c lib/strlist_push.c lib/strlist_pushb.c lib/strlist_push_tokens.c lib/str_dup.c lib/str_istr.c lib/str_len.c lib/str_ptime.c lib/str_rchr.c lib/str_tok.c lib/time_table_spd.c )
add_executable( mediathek-parser mediathek-parser.c lib/array_allocate.c lib/array_length.c lib/array_start.c lib/array_trunc.c lib/buffer_1.c lib/buffer_2.c lib/buffer_close.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_getc.c lib/buffer_getline_sa.c lib/buffer_get_token_sa.c lib/buffer_init.c lib/buffer_mmapread.c lib/buffer_munmap.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putulong.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/byte_rchr.c lib/byte_zero.c lib/fmt_long.c lib/fmt_ulong.c lib/isleap.c lib/mmap_read.c lib/open_read.c lib/stralloc_append.c lib/stralloc_catb.c lib/stralloc_catc.c lib/stralloc_cats.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_nul.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/str_chr.c lib/str_diff.c lib/str_dup.c lib/str_len.c lib/str_ptime.c lib/time_table_spd.c lib/umult64.c )
add_executable( opensearch-dump opensearch-dump.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/str_len.c )
add_executable( picc-wrapper compiler-wrapper.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_rchr.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_long.c lib/fmt_ulong.c lib/stralloc_append.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushm_internal.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/str_basename.c lib/str_diff.c lib/str_diffn.c lib/str_dup.c lib/str_len.c lib/str_rchr.c )
add_executable( picc18-wrapper compiler-wrapper.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_rchr.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_long.c lib/fmt_ulong.c lib/stralloc_append.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushm_internal.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/str_basename.c lib/str_diff.c lib/str_diffn.c lib/str_dup.c lib/str_len.c lib/str_rchr.c )
add_executable( plstest plstest.c lib/buffer_2.c lib/buffer_close.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_getc.c lib/buffer_getline_sa.c lib/buffer_get_token_sa.c lib/buffer_mmapread.c lib/buffer_munmap.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putm_internal.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putulong.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/byte_zero.c lib/fmt_ulong.c lib/mmap_read.c lib/open_read.c lib/playlist_init.c lib/playlist_m3u.c lib/playlist_read.c lib/scan_ulong.c lib/scan_ulongn.c lib/stralloc_append.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/str_diffn.c lib/str_len.c )
add_executable( rdir-test rdir-test.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putm_internal.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_zero.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fnmatch.c lib/rdir_close.c lib/rdir_open.c lib/rdir_read.c lib/stralloc_append.c lib/stralloc_catb.c lib/stralloc_catc.c lib/stralloc_cats.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/str_diff.c lib/str_len.c )
add_executable( reg2cmd reg2cmd.c lib/buffer_0.c lib/buffer_1.c lib/buffer_2.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_get_token.c lib/buffer_put.c lib/buffer_putc.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_putuint64.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/fmt_uint64.c lib/scan_fromhex.c lib/scan_xlong.c lib/scan_xlonglong.c lib/stralloc_catb.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/str_diffn.c lib/str_len.c )
add_executable( regfilter regfilter.c lib/buffer_0.c lib/buffer_1.c lib/buffer_2.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_get_token.c lib/buffer_put.c lib/buffer_putc.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_putuint64.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_chr.c lib/byte_copy.c lib/fmt_uint64.c lib/scan_fromhex.c lib/scan_xlong.c lib/scan_xlonglong.c lib/stralloc_catb.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_zero.c lib/str_diffn.c lib/str_len.c )
add_executable( sdcc-wrapper compiler-wrapper.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_rchr.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_long.c lib/fmt_ulong.c lib/stralloc_append.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushm_internal.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/str_basename.c lib/str_diff.c lib/str_diffn.c lib/str_dup.c lib/str_len.c lib/str_rchr.c )
add_executable( torrent-progress torrent-progress.c lib/buffer_1.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_get.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putulong.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/byte_copy.c lib/fmt_ulong.c lib/mmap_map.c lib/mmap_unmap.c lib/open_read.c lib/str_len.c )
add_executable( xc8-wrapper compiler-wrapper.c lib/buffer_1.c lib/buffer_2.c lib/buffer_flush.c lib/buffer_put.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putnlflush.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_stubborn.c lib/byte_copy.c lib/byte_rchr.c lib/dir_close.c lib/dir_open.c lib/dir_read.c lib/dir_type.c lib/fmt_long.c lib/fmt_ulong.c lib/stralloc_append.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_cats.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushm_internal.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/str_basename.c lib/str_diff.c lib/str_diffn.c lib/str_dup.c lib/str_len.c lib/str_rchr.c )
add_executable( xmltest xmltest.c )
add_executable( xmltest2 xmltest2.c )
add_executable( xmltest3 xmltest3.c )

##add_executable( eagle-init-brd eagle-init-brd.c )
#target_link_libraries( eagle-init-brd hmap scan stralloc str byte ${LIBXML2_LIBRARIES} m )
#set_target_properties( eagle-init-brd PROPERTIES COMPILE_FLAGS ${LIBXML2_CFLAGS} )
#
##add_executable( count-depth count-depth.c )
#target_link_libraries( count-depth buffer str fmt byte )
#
##add_executable( decode-ls-lR decode-ls-lR.c )
#target_link_libraries( decode-ls-lR stralloc str byte buffer )
#
##add_executable( httptest httptest.c )
#target_link_libraries( httptest http socket uint16 scan ndelay io iarray taia stralloc buffer fmt str byte buffer )
#
##add_executable( list-r list-r.c )
#target_link_libraries( list-r stralloc buffer str fmt dir array byte )
#
##add_executable( mediathek-list mediathek-list.c )
#target_link_libraries( mediathek-list buffer strlist stralloc str scan fmt byte )
#
##add_executable( mediathek-parser mediathek-parser.c )
#target_link_libraries( mediathek-parser buffer stralloc str mmap open fmt array byte )
#
###add_executable( picc-wrapper picc-wrapper.c )
##target_link_libraries( picc-wrapper strlist stralloc str fmt dir byte buffer )
##
###add_executable( picc18-wrapper picc18-wrapper.c )
##target_link_libraries( picc18-wrapper strlist stralloc str fmt dir byte buffer )
#
##add_executable( rdir-test rdir-test.c )
#target_link_libraries( rdir-test rdir dir buffer stralloc str byte )
#
##add_executable( reg2cmd reg2cmd.c )
#target_link_libraries( reg2cmd stralloc str buffer scan fmt byte )
#
##add_executable( regfilter regfilter.c )
#target_link_libraries( regfilter stralloc str buffer scan fmt byte )
#
###add_executable( sdcc-wrapper sdcc-wrapper.c )
##target_link_libraries( sdcc-wrapper strlist stralloc str fmt dir byte buffer )
#
##add_executable( torrent-progress torrent-progress.c )
#target_link_libraries( torrent-progress buffer mmap open fmt str byte )
#
###add_executable( xc8-wrapper xc8-wrapper.c )
##target_link_libraries( xc8-wrapper strlist stralloc str fmt dir byte buffer )
#
# 
target_link_libraries(plstest ${LIBXML2_LIBRARIES})
target_link_libraries(xmltest ${LIBXML2_LIBRARIES})
target_link_libraries(xmltest2 ${LIBXML2_LIBRARIES})
target_link_libraries(xmltest3 ${LIBXML2_LIBRARIES})
 
 
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(X64 "64")
endif()


foreach(PROGRAM ${PROGRAMS})
  # set_target_properties(${PROGRAM} PROPERTIES OUTPUT_NAME "${PROGRAM}${X64}")
  install(TARGETS ${PROGRAM} DESTINATION bin) 
endforeach(PROGRAM)

  string(REGEX REPLACE "/lib/xml2.lib" "/bin/xml2.dll" LIBXML2_DLL "${LIBXML2_LIBRARIES}")
  
  
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
