# find_zlib()
#
# Looks for an installed zlib. Sets ZLIB_FOUND, ZLIB_LIBRARY, ZLIB_INCLUDE_DIR
# and ZLIB_LIBRARY_DIR.
macro(find_zlib)
  message(CHECK_START "Checking for libz")
  find_path(ZLIB_INCLUDE_DIR zlib.h)
  find_library(ZLIB_LIBRARY NAMES z)

  if(ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
    set(ZLIB_FOUND TRUE)
    get_filename_component(ZLIB_LIBRARY_DIR "${ZLIB_LIBRARY}" DIRECTORY)
    set(ZLIB_LIBRARIES "${ZLIB_LIBRARY}")
    message(CHECK_PASS "${ZLIB_LIBRARY}")
  else()
    set(ZLIB_FOUND FALSE)
    unset(ZLIB_INCLUDE_DIR CACHE)
    unset(ZLIB_LIBRARY CACHE)
    message(CHECK_FAIL "NOT found")
  endif()
endmacro(find_zlib)
