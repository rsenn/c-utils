if(USE_LZMA)
  message(CHECK_START "Checking for lzma library")
  pkg_search_module(LIBLZMA liblzma lzma QUIET)

  if(LIBLZMA_FOUND)
    if(pkgcfg_lib_LIBLZMA_lzma)
      set(LIBLZMA_LIBRARY ${pkgcfg_lib_LIBLZMA_lzma} CACHE FILEPATH "")
    endif(pkgcfg_lib_LIBLZMA_lzma)
  endif(LIBLZMA_FOUND)
  #
  # if(NOT LIBLZMA_FOUND) include(FindLibLZMA) endif(NOT LIBLZMA_FOUND)

  if(NOT LIBLZMA_FOUND)
    check_library_exists(lzma lzma_auto_decoder "" HAVE_LIBLZMA)
    check_include_file(lzma.h HAVE_LZMA_H)

    if(HAVE_LIBLZMA AND HAVE_LZMA_H)
      set(LIBLZMA_FOUND TRUE)
      set(LIBLZMA_INCLUDE_DIRS "" CACHE STRING "lzma include directories")
      set(LIBLZMA_LIBRARY lzma CACHE STRING "lzma library")
      message(STATUS "Found liblzma")
      # message(STATUS "  LIBLZMA_LIBRARY=${LIBLZMA_LIBRARY}")
    endif(HAVE_LIBLZMA AND HAVE_LZMA_H)

  endif(NOT LIBLZMA_FOUND)

  if(LIBLZMA_FOUND)
    message(CHECK_PASS "found")
  else(LIBLZMA_FOUND)
    message(CHECK_FAIL "NOT found")
  endif(LIBLZMA_FOUND)


  unset(pkgcfg_lib_LIBLZMA_lzma CACHE)

  # dump(LIBLZMA_INCLUDE_DIRS LIBLZMA_LIBRARIES)

else(USE_LZMA)
  unset(LIBLZMA_LIBRARY CACHE)
  unset(LIBLZMA_INCLUDE_DIRS CACHE)
  unset(LIBLZMA_FOUND CACHE)

endif(USE_LZMA)
