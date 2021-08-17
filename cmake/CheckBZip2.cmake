if(USE_BZIP)
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # Check BZip2
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  pkg_search_module(BZIP2 libbz2 bzip2)
  if(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    message(STATUS "bzip2 package config not found")
    #[[  include(FindBZip2)

    if(BZIP2_FOUND)
      set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
      set(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIRS})
    endif(BZIP2_FOUND)]]
    find_library(BZIP2_LIBRARY NAMES bz2)
    if(BZIP2_LIBRARY)
      message(STATUS "bzip2 library found: ${BZIP2_LIBRARY}")
      set(BZIP2_FOUND TRUE)
    endif(BZIP2_LIBRARY)

  else(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    # pkg_get_variable(BZIP2_LIBRARY "${BZIP2_MODULE_NAME}")
    if(pkgcfg_lib_BZIP2_bz2)
      set(BZIP2_LIBRARY ${pkgcfg_lib_BZIP2_bz2})
    else(pkgcfg_lib_BZIP2_bz2)
      if(BZIP2_LIBRARIES)
        set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
      else(BZIP2_LIBRARIES)
        set(BZIP2_LIBRARY bz2)
      endif(BZIP2_LIBRARIES)
    endif(pkgcfg_lib_BZIP2_bz2)
    set(BZIP2_LINK_LIBRARY ${pkgcfg_lib_BZIP2_bz2} ${BZIP2_LINK_LIBRARIES})
    set(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIRS})
    set(BZIP2_LIBRARY_DIR ${BZIP2_LIBRARY_DIRS})
    message(STATUS "bzip2 package found: ${BZIP2_INCLUDE_DIR} ${BZIP2_LIBRARY}")
  endif(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")

  unset(BZIP2_LDFLAGS CACHE)
  unset(BZIP2_STATIC_LDFLAGS CACHE)
endif(USE_BZIP)
