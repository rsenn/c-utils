set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(BUILD_SHARED_LIBS FALSE)

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

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

include(FindBoost)
#include(CMakeDetermineCompileFeatures)
#include(CheckCXX11Features)

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


include(CheckIncludeFile)

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

find_package(libxml2)
include_directories(${LIBXML2_INCLUDE_DIR})
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

set(PROGRAMS list-r count-depth decode-ls-lR torrent-progress reg2cmd)
set(CXXPROGRAMS kbd-adjacency #ls-R
    piccfghex)

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

include_directories("${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/lib" "${CMAKE_BINARY_DIR}")

add_definitions(-DHAVE_CONFIG_H)

set(HAVE_VARS HAVE_ALLOCA_H HAVE_BOOST_FOREACH_HPP HAVE_INTTYPES_H HAVE_LIBGEN_H HAVE_STDINT_H HAVE_SYS_MMAN_H HAVE_SYS_STAT_H HAVE_SYS_TYPES_H)

foreach(V ${HAVE_VARS})
  if(${V})
    add_definitions(-D${V}=1)
  endif()
endforeach()

set(LIBOWFAT_SOURCES lib/array.h lib/array_allocate.c lib/array_bytes.c lib/array_cat.c lib/array_cat0.c lib/array_catb.c lib/array_cate.c lib/array_cats.c lib/array_cats0.c lib/array_equal.c lib/array_fail.c lib/array_get.c lib/array_length.c lib/array_reset.c lib/array_start.c lib/array_trunc.c lib/array_truncate.c lib/buffer.h lib/buffer_0.c lib/buffer_0small.c lib/buffer_1.c lib/buffer_1small.c lib/buffer_2.c lib/buffer_close.c lib/buffer_default.c lib/buffer_dummyread.c lib/buffer_dummyreadmmap.c lib/buffer_dump.c lib/buffer_feed.c lib/buffer_flush.c lib/buffer_free.c lib/buffer_fromarray.c lib/buffer_frombuf.c lib/buffer_fromsa.c lib/buffer_fromstr.c lib/buffer_get.c lib/buffer_getc.c lib/buffer_getline.c lib/buffer_getline_sa.c lib/buffer_getn.c lib/buffer_getnewline_sa.c lib/buffer_get_new_token_sa.c lib/buffer_get_new_token_sa_pred.c lib/buffer_get_token.c lib/buffer_get_token_pred.c lib/buffer_get_token_sa.c lib/buffer_get_token_sa_pred.c lib/buffer_get_until.c lib/buffer_init.c lib/buffer_init_free.c lib/buffer_mmapprivate.c lib/buffer_mmapread.c lib/buffer_mmapread_fd.c lib/buffer_munmap.c lib/buffer_peek.c lib/buffer_prefetch.c lib/buffer_put.c lib/buffer_put8long.c lib/buffer_putalign.c lib/buffer_putc.c lib/buffer_puterror.c lib/buffer_puterror2.c lib/buffer_putflush.c lib/buffer_putlong.c lib/buffer_putlonglong.c lib/buffer_putm_internal.c lib/buffer_putm_internal_flush.c lib/buffer_putnlflush.c lib/buffer_putnspace.c lib/buffer_puts.c lib/buffer_putsa.c lib/buffer_putsaflush.c lib/buffer_putsalign.c lib/buffer_putsflush.c lib/buffer_putspace.c lib/buffer_putuint64.c lib/buffer_putulong.c lib/buffer_putulonglong.c lib/buffer_putxlong.c lib/buffer_seek.c lib/buffer_skip_until.c lib/buffer_stubborn.c lib/buffer_stubborn2.c lib/buffer_tosa.c lib/buffer_truncfile.c lib/byte.h lib/byte_chr.c lib/byte_copy.c lib/byte_copyr.c lib/byte_diff.c lib/byte_fill.c lib/byte_rchr.c lib/byte_zero.c lib/dir.h lib/directory_iterator.cpp lib/directory_iterator.h lib/directory_iterator.hpp lib/dir_close.c lib/dir_internal.h lib/dir_name.c lib/dir_open.c lib/dir_read.c lib/dir_time.c lib/dir_type.c lib/file.cpp lib/file.hpp lib/fmt.h lib/fmt_8long.c lib/fmt_8longlong.c lib/fmt_asn1derlength.c lib/fmt_asn1dertag.c lib/fmt_double.c lib/fmt_escapecharc.c lib/fmt_escapecharhtml.c lib/fmt_escapecharjson.c lib/fmt_escapecharquotedprintable.c lib/fmt_escapecharquotedprintableutf8.c lib/fmt_escapecharxml.c lib/fmt_fill.c lib/fmt_httpdate.c lib/fmt_human.c lib/fmt_humank.c lib/fmt_iso8601.c lib/fmt_long.c lib/fmt_longlong.c lib/fmt_minus.c lib/fmt_pad.c lib/fmt_pb_double.c lib/fmt_pb_float.c lib/fmt_pb_int.c lib/fmt_pb_sint.c lib/fmt_pb_string.c lib/fmt_pb_tag.c lib/fmt_pb_type0_sint.c lib/fmt_pb_type1_double.c lib/fmt_pb_type1_fixed64.c lib/fmt_pb_type2_string.c lib/fmt_pb_type5_fixed32.c lib/fmt_pb_type5_float.c lib/fmt_plusminus.c lib/fmt_str.c lib/fmt_strm_internal.c lib/fmt_strn.c lib/fmt_tohex.c lib/fmt_uint64.c lib/fmt_ulong.c lib/fmt_ulong0.c lib/fmt_ulonglong.c lib/fmt_utf8.c lib/fmt_varint.c lib/fmt_xlong.c lib/fmt_xlonglong.c lib/fmt_xmlescape.c lib/fnmatch.c lib/fnmatch.h lib/hmap.h lib/hmap_add.c lib/hmap_add_tuple_with_data.c lib/hmap_delete.c lib/hmap_destroy.c lib/hmap_free_data.c lib/hmap_init.c lib/hmap_internal.c lib/hmap_internal.h lib/hmap_is_locate.c lib/hmap_print_list.c lib/hmap_print_table.c lib/hmap_print_tree.c lib/hmap_search.c lib/hmap_set.c lib/hmap_truncate.c lib/iarray_allocate.c lib/iarray_free.c lib/iarray_get.c lib/iarray_init.c lib/iarray_length.c lib/intelhex.cpp lib/intelhex.h lib/isleap.c lib/likely.h lib/mmap.h lib/mmap_map.c lib/mmap_private.c lib/mmap_read.c lib/mmap_read_fd.c lib/mmap_unmap.c lib/open.h lib/open_append.c lib/open_read.c lib/open_rw.c lib/open_trunc.c lib/safemult.h lib/scan.h lib/scan_double.c lib/scan_fromhex.c lib/scan_xlong.c lib/scan_xlonglong.c lib/str.h lib/stralloc.h lib/stralloc_append.c lib/stralloc_append_sa.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_catc.c lib/stralloc_catlong0.c lib/stralloc_cats.c lib/stralloc_catulong0.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_diffs.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_insertb.c lib/stralloc_move.c lib/stralloc_nul.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_remove.c lib/stralloc_trunc.c lib/stralloc_write.c lib/stralloc_zero.c lib/strlist.h lib/strlist_at.c lib/strlist_cat.c lib/strlist_count.c lib/strlist_dump.c lib/strlist_index_of.c lib/strlist_push.c lib/strlist_pushb.c lib/strlist_pushm_internal.c lib/strlist_pushsa.c lib/strlist_push_sa.c lib/strlist_push_unique.c lib/strlist_shift.c lib/strlist_sort.c lib/strlist_to_argv.c lib/strlist_unshift.c lib/strptime.c lib/str_chr.c lib/str_diff.c lib/str_diffn.c lib/str_len.c lib/time_table_spd.c lib/uint16.h lib/uint32.h lib/uint64.h lib/umult64.c lib/utf8.c lib/utf8.h)

add_library(libowfat ${LIBOWFAT_SOURCES})

if("${CMAKE_STATIC_LIBRARY_PREFIX}" STREQUAL "lib")
    set_target_properties(libowfat PROPERTIES
        ARCHIVE_OUTPUT_NAME owfat
        LIBRARY_OUTPUT_NAME owfat
    )
endif()

add_executable(list-r list-r.c) # buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_putc.c buffer_putsa.c buffer_puts.c buffer_putuint64.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c dir.h dir_close.c dir_internal.h dir_open.c dir_read.c dir_time.c dir_type.c fmt.h fmt_uint64.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_len.c stralloc.h stralloc_catb.c stralloc_cats.c stralloc_copyb.c stralloc_copys.c stralloc_init.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_zero.c

 
add_executable(count-depth count-depth.c) # buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_putc.c buffer_puts.c buffer_putulong.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c fmt.h fmt_ulong.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_len.c stralloc.h stralloc_catb.c stralloc_init.c stralloc_ready.c stralloc_readyplus.c stralloc_zero.c
add_executable(decode-ls-lR decode-ls-lR.c) # buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_putc.c buffer_puts.c buffer_putuint64.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c decode-ls-lR.c fmt.h fmt_uint64.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_len.c str_diffn.c stralloc.h stralloc_catb.c stralloc_copyb.c stralloc_copys.c  stralloc_init.c stralloc_ready.c stralloc_readyplus.c stralloc_zero.c
add_executable(torrent-progress torrent-progress.c) # buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putnlflush.c buffer_putulong.c buffer_puts.c buffer_putulong.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c fmt.h fmt_ulong.c mmap.h mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open.h open_read.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_len.c stralloc.h stralloc_catb.c stralloc_init.c stralloc_ready.c stralloc_readyplus.c stralloc_zero.c torrent-progress.c
add_executable(reg2cmd reg2cmd.c) # buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_feed.c buffer_flush.c buffer_get_token.c buffer_getc.c buffer_getline.c buffer_put.c buffer_putc.c buffer_puts.c buffer_putuint64.c buffer_stubborn.c buffer_stubborn2.c byte.h byte_chr.c byte_copy.c fmt.h fmt_uint64.c reg2cmd.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_len.c stralloc.h stralloc_catb.c stralloc_init.c stralloc_ready.c stralloc_readyplus.c stralloc_zero.c
add_executable(eagle-init-brd eagle-init-brd.c) # lib/hmap_add.c lib/hmap_add_tuple_with_data.c lib/hmap_delete.c lib/hmap_destroy.c lib/hmap_free_data.c lib/hmap_init.c lib/hmap_internal.c lib/hmap_is_locate.c lib/hmap_print_list.c lib/hmap_print_table.c lib/hmap_print_tree.c lib/hmap_search.c lib/hmap_set.c lib/hmap_truncate.c lib/scan_double.c lib/scan_fromhex.c lib/scan_xlong.c lib/scan_xlonglong.c lib/str_chr.c lib/str_diff.c lib/str_diffn.c lib/str_len.c lib/stralloc_append.c lib/stralloc_append_sa.c lib/stralloc_cat.c lib/stralloc_catb.c lib/stralloc_catc.c lib/stralloc_catlong0.c lib/stralloc_cats.c lib/stralloc_catulong0.c lib/stralloc_copy.c lib/stralloc_copyb.c lib/stralloc_copys.c lib/stralloc_diffs.c lib/stralloc_free.c lib/stralloc_init.c lib/stralloc_insertb.c lib/stralloc_move.c lib/stralloc_nul.c lib/stralloc_ready.c lib/stralloc_readyplus.c lib/stralloc_remove.c lib/stralloc_trunc.c lib/stralloc_write.c lib/stralloc_zero.c) 
target_link_libraries(eagle-init-brd ${LIBXML2_LIBRARES})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(X64 "64")
endif()

foreach(PROGRAM ${PROGRAMS})
   set_target_properties(${PROGRAM} PROPERTIES OUTPUT_NAME "${PROGRAM}${X64}")
    target_link_libraries(${PROGRAM} libowfat)
#  add_executable(${PROGRAM} "${CMAKE_SOURCE_DIR}/${PROGRAM}.c" ${COMMON_C_SOURCES})
  install(TARGETS ${PROGRAM} DESTINATION bin) 
endforeach(PROGRAM)

if(Boost_INCLUDE_DIRS)
  include_directories("${Boost_INCLUDE_DIRS}")
endif()

foreach(CXXPROGRAM ${CXXPROGRAMS})
  add_executable(${CXXPROGRAM} "${CMAKE_SOURCE_DIR}/${CXXPROGRAM}.cpp" ${COMMON_CXX_SOURCES} ${COMMON_C_SOURCES})
  install(TARGETS ${CXXPROGRAM} DESTINATION bin) 
endforeach(CXXPROGRAM)

