if(USE_BROTLI)
  pkg_search_module(BROTLIDEC libbrotlidec brotlidec QUIET)
  pkg_search_module(BROTLIENC libbrotlienc brotlienc QUIET)
  pkg_search_module(BROTLICOMMON libbrotlicommon brotlicommon QUIET)

  if(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")
    message(STATUS "brotli package config not found")

    find_library(BROTLIDEC_LIBRARY NAMES brotlidec CMAKE_FIND_ROOT_PATH_BOTH)
    find_library(BROTLIENC_LIBRARY NAMES brotlienc CMAKE_FIND_ROOT_PATH_BOTH)
    find_library(BROTLICOMMON_LIBRARY NAMES brotlicommon CMAKE_FIND_ROOT_PATH_BOTH)
    if(BROTLIDEC_LIBRARY AND BROTLIENC_LIBRARY AND BROTLICOMMON_LIBRARY)
      message(STATUS "brotli library found: ${BROTLIDEC_LIBRARY} ${BROTLIENC_LIBRARY} ${BROTLICOMMON_LIBRARY}")
      set(BROTLI_FOUND TRUE)
      set(BROTLI_LIBRARY ${BROTLIDEC_LIBRARY} ${BROTLIENC_LIBRARY} ${BROTLICOMMON_LIBRARY})

    endif(BROTLIDEC_LIBRARY AND BROTLIENC_LIBRARY AND BROTLICOMMON_LIBRARY)

  endif(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")

  if(BROTLICOMMON_FOUND OR BROTLI_FOUND)

    message(STATUS "brotli package found: ${BROTLI_PREFIX} ${BROTLI_LIBRARY_DIRS} ${BROTLI_LIBRARIES}")
    # pkg_get_variable(BROTLI_LIBRARY "${BROTLI_MODULE_NAME}")

    set(BROTLI_LIBRARY "")
    if(pkgcfg_lib_BROTLIENC_brotlienc AND EXISTS "${pkgcfg_lib_BROTLIENC_brotlienc}")
      list(APPEND BROTLI_LIBRARY "${pkgcfg_lib_BROTLIENC_brotlienc}")
    endif(pkgcfg_lib_BROTLIENC_brotlienc AND EXISTS "${pkgcfg_lib_BROTLIENC_brotlienc}")
    if(pkgcfg_lib_BROTLIENC_brotlidec AND EXISTS "${pkgcfg_lib_BROTLIENC_brotlidec}")
      list(APPEND BROTLI_LIBRARY "${pkgcfg_lib_BROTLIENC_brotlidec}")
    endif(pkgcfg_lib_BROTLIENC_brotlidec AND EXISTS "${pkgcfg_lib_BROTLIENC_brotlidec}")
    if(pkgcfg_lib_BROTLIENC_brotlicommon AND EXISTS "${pkgcfg_lib_BROTLIENC_brotlicommon}")
      list(APPEND BROTLI_LIBRARY "${pkgcfg_lib_BROTLIENC_brotlicommon}")
    endif(pkgcfg_lib_BROTLIENC_brotlicommon AND EXISTS "${pkgcfg_lib_BROTLIENC_brotlicommon}")

    set(BROTLI_LINK_LIBRARY ${pkgcfg_lib_BROTLIENC_brotlienc} ${pkgcfg_lib_BROTLIDEC_brotlidec} ${pkgcfg_lib_BROTLICOMMON_brotlicommon})
    set(BROTLI_LIBRARY_DIR ${BROTLICOMMON_LIBRARY_DIRS})
    set(BROTLI_FOUND TRUE)
  endif(BROTLICOMMON_FOUND OR BROTLI_FOUND)

  if(NOT BROTLI_INCLUDE_DIR AND BROTLICOMMON_INCLUDE_DIRS)
    set(BROTLI_INCLUDE_DIR "${BROTLICOMMON_INCLUDE_DIRS}" CACHE PATH "Brotli include dir")
  endif(NOT BROTLI_INCLUDE_DIR AND BROTLICOMMON_INCLUDE_DIRS)

  if(NOT BROTLI_LIBRARIES)
    if(BROTLI_LIBRARY)
      set(BROTLI_LIBRARIES "${BROTLI_LIBRARY}" CACHE PATH "Brotli libraries")
    else(BROTLI_LIBRARY)
      set(BROTLI_LIBRARIES "brotlienc;brotlidec;brotlicommon" CACHE PATH "Brotli libraries")
    endif(BROTLI_LIBRARY)
  endif(NOT BROTLI_LIBRARIES)

  if(NOT BROTLI_LIBRARY_DIR AND BROTLICOMMON_LIBRARY_DIRS)
    set(BROTLI_LIBRARY_DIR "${BROTLICOMMON_LIBRARY_DIRS}" CACHE PATH "Brotli library dir")
  endif(NOT BROTLI_LIBRARY_DIR AND BROTLICOMMON_LIBRARY_DIRS)

  # dump(BROTLI_INCLUDE_DIR BROTLI_LIBRARIES BROTLI_LIBRARY_DIR)

endif(USE_BROTLI)
