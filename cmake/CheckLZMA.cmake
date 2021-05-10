if(USE_LZMA)
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # Check LibLZMA
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # pkg_search_module(LIBLZMA liblzma lzma) if(LIBLZMA_FOUND) if(pkgcfg_lib_LIBLZMA_lzma) set(LIBLZMA_LIBRARY ${pkgcfg_lib_LIBLZMA_lzma} CACHE FILEPATH "") endif(pkgcfg_lib_LIBLZMA_lzma) endif(LIBLZMA_FOUND)
  #
  # if(NOT LIBLZMA_FOUND) include(FindLibLZMA) endif(NOT LIBLZMA_FOUND)

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
