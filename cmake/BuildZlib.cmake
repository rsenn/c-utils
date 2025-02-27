if(BUILD_ZLIB)
  message(STATUS "Building zlib from 3rdparty/zlib")
  set(ZLIB_VERSION "1.2.11")
  set(ZLIB_DISABLE_TESTS TRUE)
  set(ZLIB_STATIC TRUE)
  set(ZLIB_NO_INSTALL TRUE)

  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib/adler32.c")
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zlib-${ZLIB_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib")
      file(DOWNLOAD "https://zlib.net/zlib-${ZLIB_VERSION}.tar.gz"
           "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}.tar.gz"
           SHOW_PROGRESS)
    endif()

    exec_program(
      "${TAR}" ARGS
      -C "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty" -xzf
      "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}.tar.gz")

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}")
      file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}"
           "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib")
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}")

  endif()
  include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib.cmake")
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
      3rdparty/zlib/zutil.c)
  add_library(z ${ZLIB_SOURCES})
  set_target_properties(
    z PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/zlib"
                 LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/zlib")
  link_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/zlib")

  #set(ZLIB_LIBRARY_DEBUG z CACHE FILEPATH "")
  #set(ZLIB_LIBRARY_RELEASE z CACHE FILEPATH "")
  set(ZLIB_LIBRARY z CACHE STRING "" FORCE)
  set(ZLIB_LIBRARIES z CACHE STRING "" FORCE)
  set(ZLIB_LIBRARY_DIR "${CMAKE_BINARY_DIR}/zlib" CACHE PATH "zlib library dir"
                                                        FORCE)
  set(ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/zlib"
      CACHE PATH "zlib include dir" FORCE)
endif(BUILD_ZLIB)
