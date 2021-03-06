if(USE_BROTLI)
  pkg_search_module(BROTLIDEC libbrotlidec brotlidec)
  pkg_search_module(BROTLIENC libbrotlienc brotlienc)
  pkg_search_module(BROTLICOMMON libbrotlicommon brotlicommon)
  if(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")
    message(STATUS "brotli package config not found")

    find_library(BROTLIDEC_LIBRARY NAMES brotlidec)
    find_library(BROTLIENC_LIBRARY NAMES brotlienc)
    find_library(BROTLICOMMON_LIBRARY NAMES brotlicommon)
    if(BROTLIDEC_LIBRARY AND BROTLIENC_LIBRARY AND BROTLICOMMON_LIBRARY)
      message(
        STATUS
          "brotli library found: ${BROTLIDEC_LIBRARY} ${BROTLIENC_LIBRARY} ${BROTLICOMMON_LIBRARY}")
      set(BROTLI_FOUND TRUE)
      set(BROTLI_LIBRARY ${BROTLIDEC_LIBRARY} ${BROTLIENC_LIBRARY} ${BROTLICOMMON_LIBRARY})

    endif(BROTLIDEC_LIBRARY AND BROTLIENC_LIBRARY AND BROTLICOMMON_LIBRARY)

  else(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")
    message(
      STATUS "brotli package found: ${BROTLI_PREFIX} ${BROTLI_LIBRARY_DIRS} ${BROTLI_LIBRARIES}")
    # pkg_get_variable(BROTLI_LIBRARY "${BROTLI_MODULE_NAME}")
    set(BROTLI_LIBRARY ${pkgcfg_lib_BROTLIENC_brotlienc} ${pkgcfg_lib_BROTLIDEC_brotlidec}
                       ${pkgcfg_lib_BROTLICOMMON_brotlicommon})
    set(BROTLI_LINK_LIBRARY ${pkgcfg_lib_BROTLIENC_brotlienc} ${pkgcfg_lib_BROTLIDEC_brotlidec}
                            ${pkgcfg_lib_BROTLICOMMON_brotlicommon})
    set(BROTLI_INCLUDE_DIR ${BROTLICOMMON_INCLUDE_DIRS})
    set(BROTLI_LIBRARY_DIR ${BROTLICOMMON_LIBRARY_DIRS})
    set(BROTLI_FOUND TRUE)
  endif(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")

endif(USE_BROTLI)
