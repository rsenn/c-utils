if(NOT CROSS)
  if(BUILD_LZMA)
    check_type_size(int16_t INT16_T)
    check_type_size(int32_t INT32_T)
    check_type_size(int64_t INT64_T)
    check_type_size(intmax_t INTMAX_T)
    check_type_size(uint8_t UINT8_T)
    check_type_size(uint16_t UINT16_T)
    check_type_size(uint32_t UINT32_T)
    check_type_size(uint64_t UINT64_T)
    check_type_size(uintmax_t UINTMAX_T)

    #
    check_type_size(ptrdiff_t PTRDIFF_T_SIZE)
    check_type_size(size_t SIZE_T_SIZE)
    check_type_size(long LONG_SIZE)
    check_type_size("void*" VOIDP_T_SIZE)

    # DUMP(PTRDIFF_T_SIZE) DUMP(SIZE_T_SIZE) DUMP(LONG_SIZE) DUMP(VOIDP_T_SIZE)

    #
    check_type_size("short" SIZE_OF_SHORT)
    check_type_size("int" SIZE_OF_INT)
    check_type_size("long" SIZE_OF_LONG)
    check_type_size("long long" SIZE_OF_LONG_LONG)

    #
    check_type_size("unsigned short" SIZE_OF_UNSIGNED_SHORT)
    check_type_size("unsigned" SIZE_OF_UNSIGNED)
    check_type_size("unsigned long" SIZE_OF_UNSIGNED_LONG)
    check_type_size("unsigned long long" SIZE_OF_UNSIGNED_LONG_LONG)
    check_type_size("size_t" SIZE_OF_SIZE_T)
  endif(BUILD_LZMA)
  #
  check_type_size("__int64" __INT64)
  check_type_size("unsigned __int64" UNSIGNED___INT64)

  check_type_size(uintptr_t UINTPTR_T)
  if(NOT HAVE_UINTPTR_T)
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
      set(uintptr_t "uint64_t")
    else()
      set(uintptr_t "uint32_t")
    endif()
  endif()
endif(NOT CROSS)

if(BUILD_LZMA)
  message(STATUS "Building xz from 3rdparty/xz")
  set(LZMA_VERSION "5.2.5")
  set(LZMA_DISABLE_TESTS TRUE)
  set(LZMA_STATIC TRUE)
  set(LZMA_NO_INSTALL TRUE)
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/xz-${LZMA_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz")
      file(DOWNLOAD "https://distfiles.macports.org/xz/xz-${LZMA_VERSION}.tar.bz2" "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz-${LZMA_VERSION}.tar.bz2" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty" -xjf "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz-${LZMA_VERSION}.tar.bz2")
    file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz-${LZMA_VERSION}" "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz")

  endif()
  include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz.cmake")

  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz-config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/xz/config.h")

  # configure_file("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lzma-config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/xz/config.h")

  file(
    GLOB
    LIBLZMA_SOURCES
    # 3rdparty/xz/debug/*.c 3rdparty/xz/lib/*.c 3rdparty/xz/src/common/*.c
    3rdparty/xz/src/liblzma/check/check.c
    3rdparty/xz/src/liblzma/check/*_fast.c
    3rdparty/xz/src/liblzma/check/*_table.c
    3rdparty/xz/src/liblzma/check/sha256.c
    # 3rdparty/xz/src/liblzma/check/*.S
    3rdparty/xz/src/liblzma/common/*block_util.c
    3rdparty/xz/src/liblzma/common/*coder.c
    3rdparty/xz/src/liblzma/common/*coder_memusage.c
    # 3rdparty/xz/src/liblzma/common/*coder_mt.c
    3rdparty/xz/src/liblzma/common/*common.c
    3rdparty/xz/src/liblzma/common/*easy_preset.c
    3rdparty/xz/src/liblzma/common/*filter_common.c
    3rdparty/xz/src/liblzma/common/*hardware_cputhreads.c
    3rdparty/xz/src/liblzma/common/*hardware_physmem.c
    3rdparty/xz/src/liblzma/common/*index.c
    3rdparty/xz/src/liblzma/common/*index_hash.c
    3rdparty/xz/src/liblzma/common/*outqueue.c
    3rdparty/xz/src/liblzma/common/*stream_flags_common.c
    3rdparty/xz/src/liblzma/common/*vli_size.c
    3rdparty/xz/src/liblzma/delta/*.c
    3rdparty/xz/src/liblzma/lz/*.c
    3rdparty/xz/src/liblzma/lzma/*.c
    3rdparty/xz/src/liblzma/rangecoder/*.c
    3rdparty/xz/src/liblzma/simple/*.c)
  # list(FILTER LIBLZMA_SOURCES EXCLUDE REGEX ".*_mt\\..*")

  if(LIBLZMA_SOURCES)
    add_library(lzma ${LIBLZMA_SOURCES})
    set_target_properties(lzma PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/xz" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/xz")
    set(LZMA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz")
    set(LIBLZMA_INCLUDE_DIRS ${LZMA_SOURCE_DIR}/src/common ${LZMA_SOURCE_DIR}/src/liblzma/lzma ${LZMA_SOURCE_DIR}/src/liblzma/lz ${LZMA_SOURCE_DIR}/src/liblzma/check ${LZMA_SOURCE_DIR}/src/liblzma/simple ${LZMA_SOURCE_DIR}/src/liblzma/delta ${LZMA_SOURCE_DIR}/src/liblzma/rangecoder ${LZMA_SOURCE_DIR}/src/liblzma/common ${LZMA_SOURCE_DIR}/src/liblzma/api ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/xz)
    define_have(LIBLZMA_DEFS HAVE_STDINT_H HAVE_STDLIB_H HAVE_STDBOOL_H HAVE_INTTYPES_H HAVE_LIMITS_H HAVE_STRING_H HAVE_STRINGS_H HAVE_MEMORY_H HAVE_SYS_PARAM_H HAVE_SYS_TIME_H HAVE_SYS_TYPES_H HAVE_LZMA_H)
    list(APPEND LIBLZMA_DEFS HAVE_CONFIG_H)

    # message("LIBLZMA_DEFS: ${LIBLZMA_DEFS}")
    set_target_properties(lzma PROPERTIES COMPILE_DEFINITIONS "${LIBLZMA_DEFS}" INCLUDE_DIRECTORIES "${LIBLZMA_INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/lib;${CMAKE_CURRENT_BINARY_DIR}/3rdparty/xz")
  endif(LIBLZMA_SOURCES)

  link_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/xz")
  set(LIBLZMA_LIBRARY lzma CACHE FILEPATH "")
  set(LIBLZMA_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz/src/liblzma/api" CACHE FILEPATH "")
endif(BUILD_LZMA)
