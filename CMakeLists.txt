set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_WARN_VS8 OFF)

project(dirlist C)

option(USE_LZMA "lzma compression" ON)
option(USE_BZIP "bzip2 compression" ON)
option(USE_ZLIB "zlib compression" ON)
option(USE_SSL "ssl encryption" ON)

option(USE_SELECT "Use select() syscall" ON)
option(USE_POLL "Use poll() syscall" OFF)
option(USE_EPOLL  "Use epoll() syscall" OFF)


set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)
set(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL)


#if(EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
#  add_subdirectory(3rdparty/openssl)
#endif()

set(BUILD_SHARED_LIBS FALSE)

if(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS FALSE)
else(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS TRUE)
endif(NOT ENABLE_SHARED)

include(FindPkgConfig)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFile)
include(CheckCCompilerFlag)

function(DUMP VAR)
  message("\n\nVariable dump of: " ${ARGV} "\n")

  foreach(VAR ${ARGV})
  message("\t${VAR} = ${${VAR}}")
  endforeach(VAR ${ARGV})
  message("\n")
endfunction(DUMP VAR)
string(REGEX REPLACE ".*/" "" COMPILER_NAME "${CMAKE_C_COMPILER}")

if(COMPILER_NAME MATCHES "em.*")
  set(EMSCRIPTEN TRUE)
  DUMP(EMSCRIPTEN EMCC_SUFFIX EMSCRIPTEN_PREFIX EMSCRIPTEN_TOOLCHAIN_PATH COMPILER_NAME  CMAKE_EXE_LINKER_FLAGS)
#  foreach(LIB BZIP ZLIB LZMA SSL)
#    set("USE_${LIB}" OFF)
#  endforeach(LIB BZIP ZLIB LZMA SSL)
  set(EMSCRIPTEN_EXE_SUFFIX "html")
endif(COMPILER_NAME MATCHES "em.*")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  check_c_compiler_flag("-O0" O_OPT_NONE)
  if(O_OPT_NONE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
  endif(O_OPT_NONE)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")


check_c_compiler_flag("-flto" F_LTO)

if(F_LTO)
  option(ENABLE_LTO "Enable link-time optimization" OFF)
endif(F_LTO)
if(EMSCRIPTEN)
  option(ENABLE_LTO "Enable link-time optimization" ON)
endif(EMSCRIPTEN)


if(ENABLE_LTO)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
endif(ENABLE_LTO)


check_function_exists(lseek64 HAVE_LSEEK64)
if(NOT HAVE_LSEEK64)
  check_function_exists(llseek HAVE_LLSEEK)
  if(NOT HAVE_LLSEEK)
    check_function_exists(lseek HAVE_LSEEK)
    if(NOT HAVE_LSEEK)
    else(NOT HAVE_LSEEK)
      add_definitions(-Dio_seek=lseek)
    endif(NOT HAVE_LSEEK)
  else(NOT HAVE_LLSEEK)
    add_definitions(-Dio_seek=llseek)
  endif(NOT HAVE_LLSEEK)
else(NOT HAVE_LSEEK64)
    add_definitions(-Dio_seek=lseek64)
endif(NOT HAVE_LSEEK64)

if(USE_POLL STREQUAL "OFF" AND NOT USE_SELECT STREQUAL "ON")
  message(INFO "Checking for poll()...")
  check_function_exists(poll_wait HAVE_POLL)
  if(HAVE_POLL)
    message(INFO "Checking for poll()... OK")
    set(USE_POLL TRUE)
    set(USE_SELECT FALSE)
  else(HAVE_POLL)
    message(INFO "Checking for poll()... NOT FOUND")
    set(USE_POLL TRUE)
  endif(HAVE_POLL)
endif(USE_POLL STREQUAL "OFF" AND NOT USE_SELECT STREQUAL "ON")

message("USE_SELECT=${USE_SELECT}")
message("USE_POLL=${USE_POLL}")
message("USE_EPOLL=${USE_EPOLL}")
if(USE_POLL STREQUAL "OFF" AND USE_SELECT STREQUAL "OFF" AND USE_EPOLL STREQUAL "OFF")
  set(USE_SELECT TRUE)
endif()
if(USE_POLL STREQUAL "ON")
  add_definitions(-DHAVE_POLL=1 -DIOPAUSE_POLL=1)
endif()

if(USE_SELECT STREQUAL "ON")
  add_definitions(-DUSE_SELECT=1 -DIOPAUSE_SELECT=1)
endif()



#message(INFO "Checking for epoll()...")
#check_function_exists(epoll_wait HAVE_EPOLL)
#if(HAVE_EPOLL)
#  message(INFO "Checking for epoll()... OK")
#  set(USE_EPOLL ON)
#  set(USE_SELECT OFF)
#else(HAVE_EPOLL)
#  message(INFO "Checking for epoll()... NOT FOUND")
#  set(USE_EPOLL OFF)
#endif(HAVE_EPOLL)

if(USE_EPOLL STREQUAL ON)
  add_definitions(-DHAVE_EPOLL=1)
endif()

check_library_exists(compat syscall "" HAVE_COMPAT_LIB)
if(HAVE_COMPAT_LIB)
  set(LIBCOMPAT compat)
endif()

check_library_exists(m roundf "" HAVE_MATH_LIB)
if(HAVE_MATH_LIB)
  set(LIBMATH m)
endif()

check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(sys/param.h HAVE_SYS_PARAM_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)


foreach(DEF HAVE_STDINT_H HAVE_STDLIB_H HAVE_STDBOOL_H HAVE_INTTYPES_H HAVE_LIMITS_H HAVE_STRING_H HAVE_STRINGS_H HAVE_MEMORY_H HAVE_SYS_PARAM_H HAVE_SYS_TIME_H HAVE_SYS_TYPES_H HAVE_LZMA_H)
  if(${${DEF}})
    set(DEFINES_H "${DEFINES_H} -D${DEF}=1")
  endif()
endforeach()
message(DEFINES_H: ${DEFINES_H})
add_definitions(${DEFINES_H})
#include(${CMAKE_SOURCE_DIR}/FindZlib.cmake)
#include(${CMAKE_SOURCE_DIR}/FindBZip2.cmake)
#include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)

include(${CMAKE_SOURCE_DIR}/build/cmake/CheckInline.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG=1 -DDEBUG_OUTPUT=1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_MINSIZEREL)
foreach(CompilerFlag ${CompilerFlags})
  unset(FLAG)
  string(REPLACE "/MD" "/MT" FLAG "${${CompilerFlag}}")
  #string(REPLACE "/MTd" "/MT" FLAG "${FLAG}")
  if(${CompilerFlag} MATCHES "DEBUG")
    set(FLAG "${FLAG} -DDEBUG=1")
  endif()
  string(REPLACE " " ";" FLAG "${FLAG}")
  list(REMOVE_DUPLICATES FLAG)
  string(REPLACE ";" " " FLAG "${FLAG}")
  unset(${CompilerFlag} CACHE)
    set(${CompilerFlag} "${FLAG}" CACHE STRING "Compiler flags ${CompilerFlag}")
  message("${CompilerFlag}: ${${CompilerFlag}}")
endforeach()

include(${CMAKE_SOURCE_DIR}/build/cmake/Werror.cmake)


if(USE_LZMA)
  ################################################################################
  ## Check LibLZMA
  ################################################################################
  #pkg_search_module(LIBLZMA liblzma lzma)
  #if(LIBLZMA_FOUND)
  #  if(pkgcfg_lib_LIBLZMA_lzma)
  #    set(LIBLZMA_LIBRARY ${pkgcfg_lib_LIBLZMA_lzma} CACHE FILEPATH "")
  #  endif(pkgcfg_lib_LIBLZMA_lzma)
  #endif(LIBLZMA_FOUND)
  #
  #if(NOT LIBLZMA_FOUND)
  #  include(FindLibLZMA)
  #endif(NOT LIBLZMA_FOUND)

  if(NOT LIBLZMA_FOUND)
    check_library_exists(lzma lzma_auto_decoder "" HAVE_LIBLZMA)
    check_include_file(lzma.h HAVE_LZMA_H)

    if(HAVE_LIBLZMA AND HAVE_LZMA_H)
      set(LIBLZMA_FOUND TRUE)
      set(LIBLZMA_INCLUDE_DIRS "")
      set(LIBLZMA_LIBRARY lzma)
      message(STATUS "Found liblzma")
      message(STATUS "  LIBLZMA_LIBRARY=${LIBLZMA_LIBRARY}")
    endif(HAVE_LIBLZMA AND HAVE_LZMA_H)

  endif(NOT LIBLZMA_FOUND)

else(USE_LZMA)
      unset(LIBLZMA_LIBRARY CACHE)
      unset(LIBLZMA_INCLUDE_DIRS CACHE)
      unset(LIBLZMA_FOUND CACHE)

endif(USE_LZMA)


if(USE_ZLIB)


  ################################################################################
  ## Check Zlib
  ################################################################################
  message(STATUS "Checking for libz")
  find_library(ZLIB z)

  message(STATUS "${ZLIB}")



  set(ZLIB_LIBRARY "${ZLIB}")


  if(ZLIB)
    set(ZLIB_FOUND TRUE)
    if(ZLIB STREQUAL "")
      set(ZLIB z)
    endif()
    set(ZLIB_LIBARY_RELEASE "${ZLIB}" CACHE FILEPATH "")
    set(ZLIB_LIBARY_DEBUG "${ZLIB}" CACHE FILEPATH "")
    set(ZLIB_LIBARY "${ZLIB}" CACHE FILEPATH "")
  endif(ZLIB)

  if(NOT ZLIB_FOUND)

    pkg_search_module(ZLIB zlib libz z)
    if(ZLIB_FOUND)
      if(pkgcfg_lib_ZLIB_z)
        set(ZLIB_LIBRARY_RELEASE "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
        set(ZLIB_LIBRARY_DEBUG "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
      endif(pkgcfg_lib_ZLIB_z)
    else(ZLIB_FOUND)
      #include(FindZlib)
    endif(ZLIB_FOUND)

  endif(NOT ZLIB_FOUND)


else(USE_ZLIB)
  unset(ZLIB_LIBRARY_RELEASE CACHE)
  unset(ZLIB_LIBRARY_DEBUG CACHE)
  unset(ZLIB_LIBRARY CACHE)
endif(USE_ZLIB)


if(USE_BZIP)
  ################################################################################
  ## Check BZip2
  ################################################################################
  pkg_search_module(BZIP2 libbz2 bzip2)
  if(NOT BZIP2_FOUND)
    include(FindBZip2)
  endif(NOT BZIP2_FOUND)
endif(USE_BZIP)

################################################################################
## Check OpenSSL
################################################################################
if(USE_SSL AND NOT BUILD_SSL)
  if(NOT OPENSSL_ROOT_DIR)
    set(OPENSSL_ROOT_DIR "${CMAKE_INSTALL_PREFIX}" CACHE FILEPATH "OpenSSL root directory")
  endif()

  pkg_search_module(OPENSSL openssl libssl)

  if(OPENSSL_FOUND)
    if(pkgcfg_lib_OPENSSL_crypto)
      set(OPENSSL_CRYPTO_LIBRARY "${pkgcfg_lib_OPENSSL_crypto}")
      list(APPEND OPENSSL_LIBRARIES ${pkgcfg_lib_OPENSSL_crypto})
    endif(pkgcfg_lib_OPENSSL_crypto)
    if(pkgcfg_lib_OPENSSL_ssl)
      set(OPENSSL_SSL_LIBRARY "${pkgcfg_lib_OPENSSL_ssl}")

      list(APPEND OPENSSL_LIBRARIES ${pkgcfg_lib_OPENSSL_ssl})
    endif(pkgcfg_lib_OPENSSL_ssl)
  else(OPENSSL_FOUND)
    include(FindOpenSSL)
  endif(OPENSSL_FOUND)


  if(OPENSSL_FOUND)
    if(NOT OPENSSL_INCLUDE_DIR)
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    endif()
     if(NOT OPENSSL_LIB_DIR)
    set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
    endif()
  endif(OPENSSL_FOUND)

  message(STATUS "OPENSSL_FOUND=${OPENSSL_FOUND}\n   OPENSSL_INCLUDE_DIR=${OPENSSL_LIB_DIR}\n   OPENSSL_LIB_DIR=${OPENSSL_INCLUDE_DIR}\n   OPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}\n   OPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}\n   OPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}")

else(USE_SSL AND NOT BUILD_SSL)
 unset(OPENSSL_LIBRARIES CACHE)
 unset(OPENSSL_CRYPTO_LIBRARY CACHE)
 unset(OPENSSL_SSL_LIBRARY CACHE)
 unset(OPENSSL_INCLUDE_DIR CACHE)
endif(USE_SSL AND NOT BUILD_SSL)
#link_libraries(
#  ${ZLIB_LIBRARY}
#  ${BZIP2_LIBRARY}
#  ${LIBLZMA_LIBRARY}
#  ${CRYPTO_LIBRARIES}
#  )
add_definitions(-Dinline=__inline)


if(MINGW OR WIN32 OR WIN64 OR MSVC)
  check_library_exists(ws2_32 select "" HAVE_WS2_32_LIB)
  if(HAVE_WS2_32_LIB)
    set(LIBMSWSOCK ws2_32)
  else(HAVE_WS2_32_LIB)
    check_library_exists(wsock32 select "" HAVE_WSOCK32_LIB)
    if(HAVE_WSOCK32_LIB)
      set(LIBMSWSOCK wsock32)
    else(HAVE_WSOCK32_LIB)
      check_library_exists(mswsock select "" HAVE_MSWSOCK_LIB)
      if(HAVE_MSWSOCK_LIB)
        set(LIBMSWSOCK mswsock)
      endif(HAVE_MSWSOCK_LIB)
    endif(HAVE_WSOCK32_LIB)
  endif(HAVE_WS2_32_LIB)
endif(MINGW OR WIN32 OR WIN64 OR MSVC)

if(USE_SELECT)
   add_definitions(-DUSE_SELECT=1)
endif(USE_SELECT)

if(WIN32 OR MSVC)
  if(USE_WIDECHAR)
     add_definitions(-DUSE_WIDECHAR=1)
  endif(USE_WIDECHAR)
endif(WIN32 OR MSVC)

if(NOT ZLIB_FOUND OR NOT EXISTS "${ZLIB_LIBRARY}")
  unset(ZLIB_FOUND CACHE)
  unset(ZLIB_LIBRARY CACHE)
endif()

if(USE_ZLIB)
  message("ZLIB_LIBRARY: ${ZLIB_LIBRARY} ZLIB_FOUND: ${ZLIB_FOUND}")
  #if(ZLIB_INCLUDE_DIR)
   # link_libraries(${ZLIB_LIBRARY})
    include_directories(${ZLIB_INCLUDE_DIR})
  #endif(ZLIB_INCLUDE_DIR)

  #if(ZLIB_LIBRARY)
    add_definitions(-DHAVE_ZLIB=1)
  #else(ZLIB_LIBRARY)
  #  add_definitions(-DHAVE_ZLIB=0)
  #endif(ZLIB_LIBRARY)
endif(USE_ZLIB)

if(USE_BZIP)
  message("BZIP2_LIBRARY: ${BZIP2_LIBRARY} BZIP2_FOUND: ${BZIP2_FOUND}")
  #if(BZIP2_LIBRARY)
    add_definitions(-DHAVE_LIBBZ2=1)
    include_directories(${BZIP2_INCLUDE_DIR})
  #else(BZIP2_LIBRARY)
  #  add_definitions(-DHAVE_LIBBZ2=0)
  #endif(BZIP2_LIBRARY)
endif(USE_BZIP)

if(USE_LZMA)
  message("LIBLZMA_LIBRARY: ${LIBLZMA_LIBRARY} LIBLZMA_FOUND: ${LIBLZMA_FOUND}")
  #if(LIBLZMA_INCLUDE_DIR)
    #link_libraries(${LIBLZMA_LIBRARY})
    include_directories(${LIBLZMA_INCLUDE_DIR})
  #endif(LIBLZMA_INCLUDE_DIR)
  #if(LIBLZMA_LIBRARY)
   add_definitions(-DHAVE_LIBLZMA=1 -DLZMA_API_STATIC=1)
  #else(LIBLZMA_LIBRARY)
  #  add_definitions(-DHAVE_LIBLZMA=0)
  #endif(LIBLZMA_LIBRARY)
endif(USE_LZMA)


macro(invert INVAR OUTVAR)
  if(${${INVAR}})
    set("${OUTVAR}" FALSE)
  else(${${INVAR}})
    set("${OUTVAR}" TRUE)
  endif(${${INVAR}})
endmacro(invert INVAR OUTVAR)


macro(invoption VAR DESC INVAR)
  if(${${INVAR}})
    option("${VAR}" "${DESC}" OFF)
  else(${${INVAR}})
    option("${VAR}" "${DESC}" ON)
  endif(${${INVAR}})
endmacro(invoption VAR DESC INVAR)


invoption(BUILD_LZMA "build 3rdparty liblzma" LIBLZMA_FOUND)
invoption(BUILD_BZIP "build 3rdparty libbz2" BZIP2_FOUND)
invoption(BUILD_ZLIB "build 3rdparty libz" ZLIB_FOUND)
invoption(BUILD_SSL "build 3rdparty openssl" OPENSSL_FOUND)


if(BUILD_ZLIB)
  message("Building zlib from 3rdparty/zlib")
  set(ZLIB_SOURCES 
    3rdparty/zlib/adler32.c
    3rdparty/zlib/compress.c
    3rdparty/zlib/crc32.c
    3rdparty/zlib/deflate.c
    3rdparty/zlib/infback.c
    3rdparty/zlib/inffast.c
    3rdparty/zlib/inflate.c
    3rdparty/zlib/inftrees.c
    3rdparty/zlib/trees.c
    3rdparty/zlib/uncompr.c
    3rdparty/zlib/zutil.c
  )
  add_library(z ${ZLIB_SOURCES})
  set(ZLIB_LIBRARY_DEBUG z CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY_RELEASE z CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY z CACHE FILEPATH "" FORCE)
  set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/zlib CACHE PATH "" FORCE)
endif(BUILD_ZLIB)

if(BUILD_BZIP)
  message("Building libbz2 from 3rdparty/bzip2")
  set(LIBBZ2_SOURCES
    3rdparty/bzip2/blocksort.c
    3rdparty/bzip2/bzlib.c
    3rdparty/bzip2/compress.c
    3rdparty/bzip2/crctable.c
    3rdparty/bzip2/decompress.c
    3rdparty/bzip2/huffman.c
    3rdparty/bzip2/randtable.c
  )
  add_library(bz2 ${LIBBZ2_SOURCES})
  set(BZIP2_LIBRARY_DEBUG bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY_RELEASE bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/bzip2 CACHE PATH "" FORCE)
endif(BUILD_BZIP)


function(DEFINE_HAVE OUTVAR)
  set(OUT "")
  foreach(DEF ${ARGN})
 # message("Check definition ${DEF}: ${${DEF}}")
    if(${DEF} STREQUAL 1)
      list(APPEND OUT "${DEF}=1")
    endif(${DEF} STREQUAL 1)
  endforeach(DEF ${ARGN})
  set("${OUTVAR}" "${OUT}" PARENT_SCOPE)

endfunction(DEFINE_HAVE)

if(BUILD_LZMA)
  message("Building liblzma from 3rdparty/xz")
  file(GLOB LIBLZMA_SOURCES
    #3rdparty/xz/debug/*.c
    #3rdparty/xz/lib/*.c
    #3rdparty/xz/src/common/*.c
    3rdparty/xz/src/liblzma/check/check.c
    3rdparty/xz/src/liblzma/check/*_fast.c
    3rdparty/xz/src/liblzma/check/*_table.c
    #3rdparty/xz/src/liblzma/check/*.S
    3rdparty/xz/src/liblzma/common/*.c
    3rdparty/xz/src/liblzma/delta/*.c
    3rdparty/xz/src/liblzma/lz/*.c
    3rdparty/xz/src/liblzma/lzma/*.c
    3rdparty/xz/src/liblzma/rangecoder/*.c
    3rdparty/xz/src/liblzma/simple/*.c
  )
  list(FILTER LIBLZMA_SOURCES EXCLUDE REGEX ".*_mt\\..*")

  add_library(lzma ${LIBLZMA_SOURCES})
  set(LZMA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/xz")
  set(LIBLZMA_INCLUDE_DIRS
    ${LZMA_SOURCE_DIR}/src/common 
    ${LZMA_SOURCE_DIR}/src/liblzma/lzma
    ${LZMA_SOURCE_DIR}/src/liblzma/lz
    ${LZMA_SOURCE_DIR}/src/liblzma/check
    ${LZMA_SOURCE_DIR}/src/liblzma/simple
    ${LZMA_SOURCE_DIR}/src/liblzma/delta
    ${LZMA_SOURCE_DIR}/src/liblzma/rangecoder
    ${LZMA_SOURCE_DIR}/src/liblzma/common 
    ${LZMA_SOURCE_DIR}/src/liblzma/api
  )
  DEFINE_HAVE(LIBLZMA_DEFS HAVE_STDINT_H HAVE_STDLIB_H HAVE_STDBOOL_H HAVE_INTTYPES_H HAVE_LIMITS_H HAVE_STRING_H HAVE_STRINGS_H HAVE_MEMORY_H HAVE_SYS_PARAM_H HAVE_SYS_TIME_H HAVE_SYS_TYPES_H HAVE_LZMA_H)
  #message("LIBLZMA_DEFS: ${LIBLZMA_DEFS}")
  set_target_properties(lzma PROPERTIES
   COMPILE_DEFINITIONS "${LIBLZMA_DEFS}"
     INCLUDE_DIRECTORIES "${LIBLZMA_INCLUDE_DIRS}"
 )
  set(LIBLZMA_LIBRARY lzma CACHE FILEPATH "" FORCE)
  set(LIBLZMA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/xz/src/liblzma/api" CACHE FILEPATH "" FORCE)
endif(BUILD_LZMA)

find_program (TAR NAMES bsdtar tar gtar gnutar star)

if(BUILD_SSL)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
      file(DOWNLOAD "https://www.openssl.org/source/openssl-1.1.1f.tar.gz" "${CMAKE_SOURCE_DIR}/openssl.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1f" "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
      exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty/openssl" -xzf "${CMAKE_SOURCE_DIR}/openssl-cmake.tar.gz")

#   file(READ "${CMAKE_SOURCE_DIR}/build/cmake/BuildOpenSSL.cmake" OPENSSL_CMAKELISTS)
#    file(WRITE "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt" "${OPENSSL_CMAKELISTS}")
#
  file(WRITE "${CMAKE_BINARY_DIR}/openssl-pthread.patch" "diff -w -r -u -- crypto/threads_pthread.c crypto/threads_pthread.c\n--- crypto/threads_pthread.c\n2020-04-11 06:10:13.642470720 +0200\n+++ crypto/threads_pthread.c\n2020-04-11 06:10:21.212373541 +0200\n@@ -45,7 +45,7 @@\n     }\n \n     pthread_mutexattr_init(&attr);\n-    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);\n+    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);\n \n     if (pthread_mutex_init(lock, &attr) != 0) {\n         pthread_mutexattr_destroy(&attr);\n")


#    file(WRITE "${CMAKE_BINARY_DIR}/openssl-poly1305_ieee754.patch" "--- 3rdparty/openssl/crypto/poly1305/poly1305_ieee754.corig     2020-02-13 09:37:58.389826334 +0100\n+++ 3rdparty/openssl/crypto/poly1305/poly1305_ieee754.c    2020-02-13 09:33:12.801691620 +0100\n@@ -104,7 +104,7 @@\n #elif defined(__mips__)\n static const u32 fcsr = 1;\n #else\n-#error \"unrecognized platform\"\n+#warning \"unrecognized platform\"\n #endif\n \n int poly1305_init(void *ctx, const unsigned char key[16])")
 
   exec_program(patch "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl" ARGS -p0 -i "${CMAKE_BINARY_DIR}/openssl-pthread.patch")
  endif() 

  add_subdirectory(3rdparty/openssl)

  set(OPENSSL_LIBRARIES ssl crypto CACHE STRING "" FORCE)
  set(OPENSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/openssl/include" CACHE FILEPATH "" FORCE)
endif()

if(USE_SSL)
  add_definitions(-DHAVE_OPENSSL=1 -DOPENSSL_API_COMPAT=0x10200000L)
  if(OPENSSL_INCLUDE_DIR)
    include_directories(${CMAKE_BINARY_DIR}/3rdparty/openssl/include ${OPENSSL_INCLUDE_DIR})
  endif()

  #if(NOT OPENSSL_LIBRARIES)
  #   if(OPENSSL_CRYPTO_LIBRARY OR OPENSSL_SSL_LIBRARY)
      set(OPENSSL_LIBARRIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
  #    endif()
  #endif()
endif(USE_SSL)


link_directories(AFTER "${CMAKE_BINARY_DIR}")

set(MODSUBDIRS alloc array binfmt buffer byte case cb cbmap charbuf coff dir dns elf env errmsg fmt gpio hashmap hmap http iarray ihex io json list map mmap ndelay omf open path pe playlist process range rdir safemult scan
  #sig
  slist socket str stralloc strarray strlist tai taia textbuf textcode uint16 uint32 uint64 unix wait xml ucs)

foreach(ARCHIVE ${MODSUBDIRS})
  file(GLOB MODHEADERS lib/${ARCHIVE}.h lib/${ARCHIVE}_*.h)
  file(GLOB MODSOURCES lib/${ARCHIVE}/*.c)
  if(NOT MODSOURCES)
  file(GLOB MODSOURCES lib/${ARCHIVE}_*.c)
  endif()
  add_library("${ARCHIVE}" ${MODSOURCES} ${MODHEADERS})
  set_target_properties("${ARCHIVE}" PROPERTIES
    PREFIX ""
    COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS}"
  )
endforeach(ARCHIVE)

foreach(NETLIB dns http io ndelay socket)
   target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()


target_link_libraries(unix stralloc buffer ndelay)
target_link_libraries(array safemult byte str)
target_link_libraries(binfmt mmap byte)
target_link_libraries(buffer alloc byte fmt mmap open str ${ZLIB_LIBRARY})
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(dns byte case fmt io open socket str taia uint16 ${LIBIPHLPAPI})
target_link_libraries(dir stralloc)
target_link_libraries(errmsg str)
target_link_libraries(elf byte str range mmap uint64 uint32)
target_link_libraries(fmt byte str uint64 uint32)
target_link_libraries(gpio mmap)
target_link_libraries(hmap str)
target_link_libraries(http dns errmsg byte io scan socket stralloc str ${OPENSSL_LIBARRIES})
target_link_libraries(io buffer iarray array byte fmt open taia ${LIBMSWSOCK})
target_link_libraries(json charbuf hmap scan)
target_link_libraries(map str)
target_link_libraries(mmap open scan ${LIBPSAPI})
target_link_libraries(open str uint32)
target_link_libraries(path unix dir strlist)
target_link_libraries(pe uint64 uint32 uint16)
target_link_libraries(playlist byte fmt scan str xml)
target_link_libraries(rdir dir)
target_link_libraries(scan byte str)
target_link_libraries(socket byte fmt ndelay scan uint16 ${LIBMSWSOCK})
target_link_libraries(str byte)
target_link_libraries(stralloc open byte fmt str)
target_link_libraries(strarray array)
target_link_libraries(strlist stralloc buffer)
target_link_libraries(taia tai)
target_link_libraries(textbuf byte playlist)
target_link_libraries(uint64 uint32)
target_link_libraries(xml buffer array strlist hmap alloc)
target_link_libraries(json slist hmap stralloc buffer alloc)
target_link_libraries(ndelay socket)
target_link_libraries(process wait errmsg)
target_link_libraries(ihex scan byte)
target_link_libraries(uint32 taia tai)

set(genmakefile_SOURCES mplab.c ini.c)

if(OPENSSL_LIB_DIR)
  set(CMAKE_EXE_LINKER_FLAGS "-L${OPENSSL_LIB_DIR}")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  option(USE_EFENCE "Enable electric fence" OFF)
endif()


if(USE_EFENCE)
  set(ELECTRICFENCE_LIBRARY efence)
endif(USE_EFENCE)

set(COMMON_LIBS unix strlist array safemult scan str byte mmap open fmt )
file(GLOB MODHEADERS lib/*.h)
function(add_program PROG)
  unset(INCLUDES)
  foreach(DEP ${ARGN})
  list(FIND MODSUBDIRS "${DEP}" I)
  if(I GREATER -1)
  list(APPEND INCLUDES "lib/${DEP}.h")
  endif(I GREATER -1)
  endforeach(DEP ${ARGN})
  #message("Program '${PROG}' dependencies: ${INCLUDES}")
  add_executable(${PROG} ${PROG}.c ${${PROG}_SOURCES} ${INCLUDES})
  target_link_libraries(${PROG} ${ARGN} ${ELECTRICFENCE_LIBRARY} ${EXTRA_LIBRARIES})
  install(TARGETS ${PROG} DESTINATION bin)
  if(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
  set_target_properties(${PROG} PROPERTIES LINK_FLAGS "-L${OPENSSL_LIB_DIR} -Wl,-rpath=${OPENSSL_LIB_DIR}")
  endif(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
  if(EMSCRIPTEN)
    set_target_properties(${PROG} PROPERTIES SUFFIX ".${EMSCRIPTEN_EXE_SUFFIX}")
  endif(EMSCRIPTEN)
endfunction(add_program PROG)



if(NOT LIBLZMA_LIBRARY)
  set(LIBLZMA_LIBRARY "${LIBLZMA_LIBRARY}")
endif(NOT LIBLZMA_LIBRARY)

set(COMPRESSION_LIBRARIES
  ${LIBLZMA_LIBRARY}
  ${ZLIB_LIBRARY}
  ${BZIP2_LIBRARY}
)

if(OPENSSL_LIBRARIES)
  set(CRYPTO_LIBRARIES ${OPENSSL_LIBRARIES} pthread ${LIBCOMPAT} )
endif(OPENSSL_LIBRARIES)


add_program(binfmttest binfmt)
if(USE_BZIP)
add_program(bsdiffcat path errmsg buffer array safemult str byte errmsg uint32 uint64 ${COMPRESSION_LIBRARIES})
add_program(ccat buffer open str unix ${COMPRESSION_LIBRARIES})
endif(USE_BZIP)
add_program(buffertest stralloc buffer array case mmap ${COMPRESSION_LIBRARIES})
add_program(compiler-wrapper stralloc buffer byte fmt str strlist dir)
add_program(count-depth buffer fmt)
add_program(decode-ls-lR stralloc buffer byte fmt io open str)
add_program(dnsip dns stralloc buffer errmsg socket ${LIBIPHLPAPI})
add_program(dnsname dns stralloc buffer errmsg ${LIBIPHLPAPI})
add_program(dnstest dns stralloc buffer errmsg ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(eagle-gen-cmds xml stralloc buffer strarray array byte cb cbmap errmsg fmt hmap mmap scan str unix ${LIBMATH})
add_program(eagle-init-brd xml stralloc buffer hmap strlist  scan str ${LIBMATH})
add_program(eagle-to-circuit xml stralloc buffer strlist array byte cb cbmap fmt hmap mmap scan str ${LIBMATH})
add_program(eagle-to-svg xml stralloc buffer strlist array byte cb cbmap fmt hmap mmap scan str)
add_program(elf64list elf mmap uint16 uint32 uint64)
add_program(elflist buffer errmsg unix elf mmap str)
add_program(elfwrsec buffer elf mmap)
add_program(genmakefile case errmsg strarray slist rdir dir path strlist hmap stralloc buffer mmap unix open scan fmt byte str array safemult alloc)
add_program(hexedit stralloc buffer array io errmsg mmap open path scan uint64 unix ${ZLIB_LIBRARY})
add_program(httptest http dns stralloc buffer errmsg byte io socket alloc taia ${LIBMSWSOCK} ${CRYPTO_LIBRARIES} pthread)
add_program(impgen buffer byte mmap open pe uint16 uint32)
add_program(jsontest json stralloc buffer byte charbuf fmt hmap mmap open)
add_program(list-r strarray strlist stralloc buffer array unix dir array fmt open str uint64 ${LIBADVAPI32})
add_program(macho32list mmap uint32)
if(USE_LZMA)
  add_program(mediathek-list errmsg stralloc buffer alloc byte fmt http scan slist str unix strarray strlist ${LIBLZMA_LIBRARY} ${OPENSSL_LIBRARIES} ${CRYPTO_LIBRARIES} pthread)
endif(USE_LZMA)

add_program(mediathek-parser ucs errmsg stralloc buffer array byte open str unix strlist)
add_program(ntldd stralloc buffer pe strlist byte path str unix uint64)
add_program(opensearch-dump xml stralloc buffer)
add_program(pathtool stralloc buffer errmsg path strlist unix)
add_program(pelist buffer errmsg mmap pe str unix)
add_program(omflist stralloc buffer errmsg omf mmap str unix)
add_program(pkgcfg stralloc buffer byte cbmap dir env errmsg path slist str unix strarray strlist)
add_program(plsconv stralloc buffer byte open playlist str unix)
add_program(rdir-test rdir stralloc buffer)
add_program(reg2cmd stralloc buffer byte fmt open scan str uint64)
add_program(regfilter stralloc buffer byte fmt open scan str uint64)
add_program(sln unix stralloc buffer byte errmsg path)
add_program(strarraytest xml dns stralloc buffer mmap strarray unix)
add_program(torrent-progress stralloc buffer io mmap open uint64)
add_program(xmlpp errmsg xml stralloc buffer hmap io unix)
add_program(xmltest xml stralloc buffer byte fmt hmap)
add_program(xmltest2 xml stralloc buffer byte fmt hmap)
add_program(xmltest3 xml stralloc buffer byte fmt hmap)
add_program(xmltest4 xml stralloc buffer array byte cb cbmap fmt hmap mmap scan str strlist)
#add_program(ziptest buffer str unix ${COMPRESSION_LIBRARIES})
add_program(cc-wrap errmsg env buffer path dir strarray strlist stralloc fmt)
add_program(ar-wrap path process errmsg env buffer dir strarray strlist stralloc fmt alloc)
add_program(cofflist coff unix errmsg stralloc buffer mmap)
add_program(msys-shell unix errmsg env buffer path strarray strlist stralloc)
add_program(tcping map unix dns case io array socket ndelay errmsg taia tai buffer stralloc mmap open fmt scan str byte uint16 ${LIBMSWSOCK} ${LIBIPHLPAPI})
add_program(crc errmsg buffer mmap open ${ZLIB_LIBRARY})
add_program(cmake-run errmsg env buffer path dir strarray strlist stralloc fmt)

add_program(jsonpp json errmsg unix)
add_program(xml2json json xml errmsg unix)
add_program(xml2moon xml textcode errmsg)
add_program(strip-comments  errmsg env charbuf buffer path dir strarray strlist stralloc fmt)

add_program(httpproxy http io map)
#add_program(montty io map)
add_program(logserial rdir dir errmsg io ndelay strarray stralloc buffer taia tai)
#add_program(tinyproxy http io)
#if(NOT WIN32)
#add_program(tcpproxy http io)
#endif(NOT WIN32)
#add_program(redir http io ${LIBCOMPAT})

add_program(parse strlist errmsg)
add_program(testihex ihex stralloc buffer alloc mmap)

add_subdirectory(tests)
add_subdirectory(examples)
