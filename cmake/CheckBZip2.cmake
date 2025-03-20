if(USE_BZIP)
  message(CHECK_START "Checking for bzip2 library")

  pkg_check_modules(BZIP2 QUIET bzip2 libbz2)
  
  if(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    message("NOT pkg-config")
    find_library(BZIP2_LIBRARY NAMES bz2)
    if(BZIP2_LIBRARY)
      set(BZIP2_FOUND TRUE)
      set(BZIP2_LIBRARIES "${BZIP2_LIBRARY}")
    endif(BZIP2_LIBRARY)

  else(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    set(BZIP2_FOUND TRUE)

    if(BZIP2_LIBRARIES)
      set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
    else(BZIP2_LIBRARIES)
      if(BZIP2_LIBRARIES)
        set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
      else(BZIP2_LIBRARIES)
        set(BZIP2_LIBRARY bz2)
      endif(BZIP2_LIBRARIES)
    endif(BZIP2_LIBRARIES)
    set(BZIP2_LINK_LIBRARY ${BZIP2_LIBRARIES} ${BZIP2_LINK_LIBRARIES})
    set(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIRS})
    set(BZIP2_LIBRARY_DIR ${BZIP2_LIBRARY_DIRS})
  endif(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
  message("BZIP2_LIBRARY_DIR: ${BZIP2_LIBRARY_DIRS}")
    message("BZIP2_INCLUDE_DIR: ${BZIP2_INCLUDE_DIR}")

  if(BZIP2_FOUND)
    message(CHECK_PASS "found")
  else(BZIP2_FOUND)
    message(CHECK_FAIL "NOT found")
  endif(BZIP2_FOUND)

  if(BZIP2_LIBRARY)
    if(NOT BZIP2_LIBRARY_DIR)
      string(REGEX REPLACE "/lib.*/.*" "/lib" BZIP2_LIBRARY_DIR
                           "${BZIP2_LIBRARY}")
    endif(NOT BZIP2_LIBRARY_DIR)
    if(NOT BZIP2_INCLUDE_DIR)
      string(REGEX REPLACE "/lib.*/.*" "/include" BZIP2_INCLUDE_DIR
                           "${BZIP2_LIBRARY}")
    endif(NOT BZIP2_INCLUDE_DIR)
  endif(BZIP2_LIBRARY)

  set(BZIP2_LIBRARY "${BZIP2_LIBRARY}" CACHE FILEPATH "bzip2 library")
  set(BZIP2_INCLUDE_DIR "${BZIP2_INCLUDE_DIR}" CACHE FILEPATH
                                                     "bzip2 include directory")
  set(BZIP2_LIBRARY_DIR "${BZIP2_LIBRARY_DIR}" CACHE FILEPATH
                                                     "bzip2 library directory")

else(USE_BZIP)
  unset(BZIP2_LDFLAGS CACHE)
  unset(BZIP2_STATIC_LDFLAGS CACHE)
endif(USE_BZIP)
