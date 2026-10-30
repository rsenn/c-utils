# find_brotli()
#
# Looks for installed brotli libraries. Sets BROTLI_FOUND, BROTLI_LIBRARIES
# (enc, dec, common), BROTLI_INCLUDE_DIR and BROTLI_LIBRARY_DIR.
macro(find_brotli)
  message(CHECK_START "Checking for brotli")
  find_path(BROTLI_INCLUDE_DIR brotli/decode.h)
  find_library(BROTLIENC_LIBRARY NAMES brotlienc)
  find_library(BROTLIDEC_LIBRARY NAMES brotlidec)
  find_library(BROTLICOMMON_LIBRARY NAMES brotlicommon)

  if(BROTLI_INCLUDE_DIR AND BROTLIENC_LIBRARY AND BROTLIDEC_LIBRARY AND BROTLICOMMON_LIBRARY)
    set(BROTLI_FOUND TRUE)
    set(BROTLI_LIBRARIES "${BROTLIENC_LIBRARY};${BROTLIDEC_LIBRARY};${BROTLICOMMON_LIBRARY}")
    get_filename_component(BROTLI_LIBRARY_DIR "${BROTLICOMMON_LIBRARY}" DIRECTORY)
    message(CHECK_PASS "${BROTLICOMMON_LIBRARY}")
  else()
    set(BROTLI_FOUND FALSE)
    unset(BROTLI_INCLUDE_DIR CACHE)
    message(CHECK_FAIL "NOT found")
  endif()
endmacro(find_brotli)
