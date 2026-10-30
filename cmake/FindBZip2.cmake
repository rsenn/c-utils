# find_bzip2()
#
# Looks for an installed bzip2. Sets BZIP2_FOUND, BZIP2_LIBRARY,
# BZIP2_INCLUDE_DIR and BZIP2_LIBRARY_DIR.
macro(find_bzip2)
  message(CHECK_START "Checking for libbz2")
  find_path(BZIP2_INCLUDE_DIR bzlib.h)
  find_library(BZIP2_LIBRARY NAMES bz2)

  if(BZIP2_INCLUDE_DIR AND BZIP2_LIBRARY)
    set(BZIP2_FOUND TRUE)
    get_filename_component(BZIP2_LIBRARY_DIR "${BZIP2_LIBRARY}" DIRECTORY)
    set(BZIP2_LIBRARIES "${BZIP2_LIBRARY}")
    message(CHECK_PASS "${BZIP2_LIBRARY}")
  else()
    set(BZIP2_FOUND FALSE)
    unset(BZIP2_INCLUDE_DIR CACHE)
    unset(BZIP2_LIBRARY CACHE)
    message(CHECK_FAIL "NOT found")
  endif()
endmacro(find_bzip2)
