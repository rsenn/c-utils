set(CMAKE_LEGACY_CYGWIN_WIN32 0) 
cmake_minimum_required(VERSION 2.8)

project(dirlist)

include(CheckIncludeFile)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)

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

set(COMMON_SOURCES dir.h buffer.h buffer_close.c buffer_default.c buffer_dump.c buffer_feed.c buffer_flush.c buffer_free.c buffer_fromsa.c buffer_fromstr.c buffer_get.c buffer_get_new_token_sa.c buffer_get_new_token_sa_pred.c buffer_get_token.c buffer_get_token_pred.c buffer_get_token_sa.c buffer_get_token_sa_pred.c buffer_get_until.c buffer_getc.c buffer_getline.c buffer_getline_sa.c buffer_init.c buffer_mmapprivate.c buffer_mmapread.c buffer_mmapread_fd.c buffer_prefetch.c buffer_put.c buffer_putc.c buffer_putflush.c buffer_putm_internal.c buffer_putnlflush.c buffer_putnspace.c buffer_puts.c buffer_putsa.c buffer_putsflush.c buffer_putspace.c buffer_putulong.c buffer_skip_until.c buffer_stubborn.c buffer_stubborn2.c buffer_tosa.c buffer_truncfile.c byte.h byte_chr.c byte_copy.c byte_copyr.c byte_fill.c dir_read.c dir_close.c dir_internal.h dir_open.c dir_time.c dir_type.c fmt.h fmt_minus.c fmt_ulong.c fmt_ulong0.c mmap.h mmap_map.c mmap_private.c mmap_read.c open.h mmap_read_fd.c open_read.c open_trunc.c scan.h shell.h shell_alloc.c shell_error.c shell_errorn.c shell_init.c shell_realloc.c str.h str_len.c stralloc.h stralloc_append.c stralloc_cat.c stralloc_catb.c stralloc_catc.c stralloc_catlong0.c stralloc_cats.c stralloc_catulong0.c stralloc_copy.c stralloc_copyb.c stralloc_copys.c stralloc_diffs.c stralloc_free.c stralloc_init.c stralloc_insertb.c stralloc_move.c stralloc_nul.c stralloc_ready.c stralloc_readyplus.c stralloc_remove.c stralloc_trunc.c stralloc_write.c stralloc_zero.c mmap_unmap.c directory_iterator.cpp directory_iterator.h directory_iterator.hpp file.cpp file.hpp open_append.c open_rw.c str_diffn.c)

set(PROGRAMS list-r count-depth decode-ls-lR torrent-progress)

include_directories("${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

add_definitions(-DHAVE_CONFIG_H)

foreach(PROGRAM ${PROGRAMS})


  add_executable(${PROGRAM} ${PROGRAM}.c ${COMMON_SOURCES})

  install(TARGETS ${PROGRAM} DESTINATION bin) 
endforeach(PROGRAM)

