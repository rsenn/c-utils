# find_lzma()
#
# Looks for an installed liblzma. Sets LIBLZMA_FOUND, LIBLZMA_LIBRARY,
# LIBLZMA_INCLUDE_DIR and LIBLZMA_LIBRARY_DIR.
macro(find_lzma)
  message(CHECK_START "Checking for liblzma")
  find_path(LIBLZMA_INCLUDE_DIR lzma.h)
  find_library(LIBLZMA_LIBRARY NAMES lzma)

  if(LIBLZMA_INCLUDE_DIR AND LIBLZMA_LIBRARY)
    set(LIBLZMA_FOUND TRUE)
    get_filename_component(LIBLZMA_LIBRARY_DIR "${LIBLZMA_LIBRARY}" DIRECTORY)
    set(LIBLZMA_LIBRARIES "${LIBLZMA_LIBRARY}")
    message(CHECK_PASS "${LIBLZMA_LIBRARY}")
  else()
    set(LIBLZMA_FOUND FALSE)
    unset(LIBLZMA_INCLUDE_DIR CACHE)
    unset(LIBLZMA_LIBRARY CACHE)
    message(CHECK_FAIL "NOT found")
  endif()
endmacro(find_lzma)
