if(USE_ZLIB)

  if(BUILD_ZLIB)
    set(ZLIB_LIBRARY "z" CACHE FILEPATH "zlib library")
    set(ZLIB_LIBRARY_DIR "${CMAKE_BINARY_DIR}/zlib")
    set(ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/zlib")
  else(BUILD_ZLIB)
    message(CHECK_START "Checking for libz")

    if(NOT ZLIB_LIBRARY)
      pkg_search_module(ZLIB zlib libz z)
      message(STATUS "Found zlib: ${pkgcfg_lib_ZLIB_z}")
      if(pkgcfg_lib_ZLIB_z)
        set(ZLIB_LIBRARY "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
        set(ZLIB_FOUND TRUE)
      elseif(ZLIB_LIBRARIES)
        set(ZLIB_LIBRARY "${ZLIB_LIBRARIES}" CACHE FILEPATH "")
        set(ZLIB_FOUND TRUE)
      endif(ZLIB_LIBRARIES)

      if(ZLIB_INCLUDE_DIRS)
        set(ZLIB_INCLUDE_DIR "${ZLIB_INCLUDE_DIRS}" CACHE PATH)
      endif(ZLIB_INCLUDE_DIRS)
      #if(ZLIB_LIBRARY_DIRS)
      #  set(ZLIB_LIBRARY_DIR "${ZLIB_LIBRARY_DIRS}" CACHE PATH)
      #endif(ZLIB_LIBRARY_DIRS)
    endif(NOT ZLIB_LIBRARY)

    if(NOT ZLIB_FOUND)
      find_library(ZLIB_LIBRARY z)

      if(ZLIB_LIBRARY)
        message(STATUS "Found libz: ${ZLIB_LIBRARY}")
        set(ZLIB_FOUND TRUE)
        if(NOT "${ZLIB_LIBRARY}" STREQUAL "")
          set(ZLIB_LIBRARIES "${ZLIB_LIBRARY}")
        endif(NOT "${ZLIB_LIBRARY}" STREQUAL "")
      endif(ZLIB_LIBRARY)
    endif(NOT ZLIB_FOUND)
    if(EXISTS "${ZLIB_LIBRARY}")
      get_filename_component(ZLIB_LIBRARY_DIR "${ZLIB_LIBRARY}" DIRECTORY)
    endif(EXISTS "${ZLIB_LIBRARY}")
    if(ZLIB_FOUND)
      message(CHECK_PASS "found")
    else(ZLIB_FOUND)
      message(CHECK_FAIL "NOT found")
    endif(ZLIB_FOUND)
  endif(BUILD_ZLIB)

  #dump(BUILD_ZLIB ZLIB_LIBRARY ZLIB_LIBRARY_DIR ZLIB_INCLUDE_DIR)

else(USE_ZLIB)
  unset(ZLIB_LIBRARY CACHE)
endif(USE_ZLIB)
