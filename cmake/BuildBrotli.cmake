if(USE_BROTLI AND NOT BROTLI_FOUND)
  set(BUILD_BROTLI ON)
endif(USE_BROTLI AND NOT BROTLI_FOUND)

if(BUILD_BROTLI)
  message(STATUS "Building brotli from 3rdparty/brotli")
  set(BROTLI_VERSION "1.0.9")
  set(BROTLI_DISABLE_TESTS TRUE)
  set(BROTLI_STATIC TRUE)
  set(BROTLI_NO_INSTALL TRUE)

  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/brotli/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/brotli")
      file(DOWNLOAD "https://github.com/google/brotli/archive/v${BROTLI_VERSION}.tar.gz"
           "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf
                               "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}.tar.gz")
    if(EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}")
      file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/brotli")
    endif()

    file(COPY "${CMAKE_SOURCE_DIR}/3rdparty/brotli.cmake"
         DESTINATION "${CMAKE_SOURCE_DIR}/3rdparty/brotli/CMakeLists.txt")

  endif()

  set(BROTLI_STATIC TRUE)
  set(BROTLI_NO_INSTALL TRUE)

  set(BROTLI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/brotli/cSs/include" CACHE PATH "brotli include directory")

  set(LIBM_LIBRARY "${LIBMATH}")

  add_subdirectory(3rdparty/brotli)
endif(BUILD_BROTLI)
