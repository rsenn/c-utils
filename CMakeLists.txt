set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(BUILD_SHARED_LIBS FALSE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")


#if(MSVC)
#  set(CMAKE_CXX_STANDARD_LIBRARIES "kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib")
#  set(CMAKE_C_STANDARD_LIBRARIES "kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib")
#endif(MSVC)
#
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


if(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
  set(USE_READDIR TRUE)
else(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)
  set(USE_READDIR FALSE)
endif(HAVE_OPENDIR_FUNC AND HAVE_READDIR_FUNC)

if(USE_READDIR)
  add_definitions(-DUSE_READDIR=1)
endif(USE_READDIR)

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

add_definitions(-DHAVE_CONFIG_H=1)

set(HAVE_VARS HAVE_ALLOCA_H HAVE_BOOST_FOREACH_HPP HAVE_INTTYPES_H HAVE_LIBGEN_H HAVE_STDINT_H HAVE_SYS_MMAN_H HAVE_SYS_STAT_H HAVE_SYS_TYPES_H)

foreach(V ${HAVE_VARS})
  if(${V})
    add_definitions(-D${V}=1)
  endif()
endforeach()

set(LIBOWFAT_SOURCES array.h array_allocate.c array_bytes.c array_cat.c array_cat0.c array_catb.c array_cate.c array_cats.c array_cats0.c array_equal.c array_fail.c array_get.c array_length.c array_reset.c array_start.c array_trunc.c array_truncate.c buffer.h buffer_0.c buffer_1.c buffer_2.c buffer_close.c buffer_default.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putuint64.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte.h byte_chr.c byte_copy.c byte_copyr.c byte_diff.c byte_fill.c byte_zero.c dir.h dir_close.c dir_internal.h dir_open.c dir_read.c dir_time.c dir_type.c directory_iterator.h fmt.h fmt_minus.c fmt_int64.c fmt_uint64.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap.h mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open.h open_append.c open_read.c open_rw.c open_trunc.c scan.h scan_fromhex.c scan_xlong.c scan_xlonglong.c str.h str_diffn.c str_len.c stralloc.h stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c strlist.h strlist_append.c strlist_append_unique.c strlist_at.c strlist_count.c strlist_index_of.c strlist_sort.c
word.h word_chr.c word_copy.c word_copyr.c word_diff.c word_fill.c word_zero.c

wstr.h wstr_diffn.c wstr_len.c wstralloc.h wstralloc_append.c wstralloc_cat.c wstralloc_catb.c wstralloc_catc.c wstralloc_catlong0.c wstralloc_cats.c wstralloc_catulong0.c wstralloc_copy.c wstralloc_copyb.c wstralloc_copys.c wstralloc_diffs.c wstralloc_free.c wstralloc_init.c wstralloc_insertb.c wstralloc_move.c wstralloc_nul.c wstralloc_ready.c wstralloc_readyplus.c wstralloc_remove.c wstralloc_trunc.c wstralloc_write.c wstralloc_zero.c )



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

if(WIN32)
  set(EXTRA_LIBS advapi32.lib)
endif(WIN32)

foreach(PROGRAM ${PROGRAMS})
  target_link_libraries(${PROGRAM} libowfat ${EXTRA_LIBS})
    
#  add_executable(${PROGRAM} "${CMAKE_SOURCE_DIR}/${PROGRAM}.c" ${COMMON_SOURCES})
  install(TARGETS ${PROGRAM} DESTINATION bin) 
endforeach(PROGRAM)

if(Boost_INCLUDE_DIRS)
  include_directories("${Boost_INCLUDE_DIRS}")
endif()

foreach(CXXPROGRAM ${CXXPROGRAMS})
  add_executable(${CXXPROGRAM} "${CMAKE_SOURCE_DIR}/${CXXPROGRAM}.cpp" ${COMMON_SOURCES})
  install(TARGETS ${CXXPROGRAM} DESTINATION bin) 
endforeach(CXXPROGRAM)

