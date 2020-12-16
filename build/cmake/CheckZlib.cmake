
if(USE_ZLIB)

  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # Check Zlib
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  message(STATUS "Checking for libz")
  find_library(ZLIB z)

  message(STATUS "${ZLIB}")

  set(ZLIB_LIBRARY "${ZLIB}")

  if(ZLIB)
    set(ZLIB_FOUND TRUE)
    if(ZLIB STREQUAL "")
      set(ZLIB z)
    endif(ZLIB STREQUAL "")
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
      # include(FindZlib)
    endif(ZLIB_FOUND)

  endif(NOT ZLIB_FOUND)

else(USE_ZLIB)
  unset(ZLIB_LIBRARY_RELEASE CACHE)
  unset(ZLIB_LIBRARY_DEBUG CACHE)
  unset(ZLIB_LIBRARY CACHE)
endif(USE_ZLIB)
