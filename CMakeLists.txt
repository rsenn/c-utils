set(CMAKE_LEGACY_CYGWIN_WIN32 0) 
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

include(FindBoost)
#include(CMakeDetermineCompileFeatures)
include(CheckCXX11Features)
if(HAS_CXX11_AUTO AND HAS_CXX11_NULLPTR AND HAS_CXX11_LAMBDA AND HAS_CXX11_STATIC_ASSERT AND HAS_CXX11_RVALUE_REFERENCES AND HAS_CXX11_DECLTYPE AND HAS_CXX11_CSTDINT_H AND HAS_CXX11_LONG_LONG)
  message(STATUS "Enabling C++11 support")
  set(CXX11 TRUE)
endif()


include(CheckIncludeFile)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(boost/foreach.hpp HAVE_BOOST_FOREACH_HPP)
check_include_file(xtr1common HAVE_XTR1COMMON)

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

configure_file("${CMAKE_SOURCE_DIR}/config.h.cmake"
               "${CMAKE_BINARY_DIR}/config.h")

set(COMMON_SOURCES buffer_close.c buffer_default.c buffer_dump.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putulong.c buffer_putulonglong.c buffer_putxlong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte_chr.c byte_copy.c byte_copyr.c byte_fill.c dir_close.c dir_open.c dir_read.c dir_time.c dir_type.c directory_iterator.cpp fmt_minus.c fmt_ulong.c fmt_ulong0.c fmt_ulonglong.c fmt_xlong.c fmt_xlonglong.c mmap_map.c mmap_private.c mmap_read.c mmap_read_fd.c mmap_unmap.c open_append.c open_read.c open_rw.c open_trunc.c scan_fromhex.c scan_xlong.c scan_xlonglong.c shell_alloc.c shell_error.c shell_errorn.c shell_init.c shell_realloc.c str_diffn.c str_len.c stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c buffer.h byte.h dir.h dir_internal.h directory_iterator.h fmt.h mmap.h open.h scan.h shell.h str.h stralloc.h uint64.h)

set(PROGRAMS list-r count-depth decode-ls-lR torrent-progress reg2cmd)
set(CXXPROGRAMS kbd-adjacency)

if(MSVC)
#  add_definitions(-Dinline=__force_inline)
  add_definitions(/DINLINE=__inline)
  add_definitions(/wo4005)
  add_definitions(/W0)
else()
  add_definitions(-DINLINE=inline)
endif()

if(CXX11)
  add_definitions(-DCXX11=1)
endif()

include_directories("${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_definitions(-DHAVE_CONFIG_H)

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

