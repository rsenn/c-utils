# ##############################################################################################################################################################################################################################################################################################################################################################################################################
# Check OpenSSL
# ##############################################################################################################################################################################################################################################################################################################################################################################################################
if(USE_SSL AND NOT BUILD_SSL)
  message(CHECK_START "Checking for openssl library")
  if(NOT OPENSSL_ROOT_DIR)
    if("$ENV{OPENSSL_ROOT}")
      set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT}" CACHE FILEPATH
                                                      "OpenSSL root directory")
      # else("$ENV{OPENSSL_ROOT}") set(OPENSSL_ROOT_DIR "${CMAKE_INSTALL_PREFIX}" CACHE FILEPATH "OpenSSL root directory")
    endif("$ENV{OPENSSL_ROOT}")
  endif(NOT OPENSSL_ROOT_DIR)
  set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
  # unset(OPENSSL_FOUND CACHE) message(STATUS "Searching pkgconfig database for openssl...")

  if(OPENSSL_ROOT_DIR)
    if(NOT EXISTS "${OPENSSL_ROOT_DIR}")
      message(
        FATAL_ERROR
          "--- FATAL error\nOPENSSL_ROOT_DIR '${OPENSSL_ROOT_DIR}' inexistent!\n--- FATAL error"
      )
    endif(NOT EXISTS "${OPENSSL_ROOT_DIR}")
    list(APPEND CMAKE_PREFIX_PATH "${OPENSSL_ROOT_DIR}")
  endif(OPENSSL_ROOT_DIR)
  include(FindPkgConfig)

  # pkg_search_module(OPENSSL openssl libssl QUIET)
  #  message(STATUS "pkg_search_module(OPENSSL openssl libssl): ${OPENSSL_FOUND}")
  find_package(OpenSSL)
  #message(STATUS "OpenSSL_DIR=${OpenSSL_DIR}")

  if(NOT OPENSSL_ROOT_DIR)
    pkg_check_modules(OPENSSL REQUIRED openssl libssl)
    message(
      STATUS "pkg_check_modules(OPENSSL openssl libssl): ${OPENSSL_FOUND}")

    find_library(pkgcfg_lib_OPENSSL_crypto crypto ${OPENSSL_LIBRARY_DIRS})
    find_library(pkgcfg_lib_OPENSSL_ssl ssl ${OPENSSL_LIBRARY_DIRS})

    if(pkgcfg_lib_OPENSSL_crypto)
      set(OPENSSL_CRYPTO_LIBRARY "${pkgcfg_lib_OPENSSL_crypto}"
          CACHE FILEPATH "OpenSSL crypto library")
    else(pkgcfg_lib_OPENSSL_crypto)
      set(OPENSSL_CRYPTO_LIBRARY "crypto" CACHE FILEPATH
                                                "OpenSSL crypto library")
    endif(pkgcfg_lib_OPENSSL_crypto)

    if(pkgcfg_lib_OPENSSL_ssl)
      set(OPENSSL_SSL_LIBRARY "${pkgcfg_lib_OPENSSL_ssl}"
          CACHE FILEPATH "OpenSSL ssl library")
    else(pkgcfg_lib_OPENSSL_ssl)
      set(OPENSSL_SSL_LIBRARY "ssl" CACHE FILEPATH "OpenSSL ssl library")
    endif(pkgcfg_lib_OPENSSL_ssl)
  else(NOT OPENSSL_ROOT_DIR)
    set(OPENSSL_FOUND TRUE)
  endif(NOT OPENSSL_ROOT_DIR)

  set(OPENSSL_LIBRARIES "${OPENSSL_SSL_LIBRARY};${OPENSSL_CRYPTO_LIBRARY}"
      CACHE FILEPATH "OpenSSL libraries")

  #dump(OPENSSL_LIBRARIES OPENSSL_LIBRARY_DIRS OPENSSL_INCLUDE_DIR)

  if(OPENSSL_FOUND)
    # message(STATUS "Found openssl at ${OPENSSL_LIBRARY_DIRS} ${OPENSSL_INCLUDE_DIRS} ...")

    if(OPENSSL_CRYPTO_LIBRARY AND OPENSSL_SSL_LIBRARY)
      set(TLS_LIBRARIES "${OPENSSL_SSL_LIBRARY};${OPENSSL_CRYPTO_LIBRARY}"
          CACHE STRING "TLS libraries")
    endif(OPENSSL_CRYPTO_LIBRARY AND OPENSSL_SSL_LIBRARY)

    list(APPEND CMAKE_REQUIRED_INCLUDES "${OPENSSL_INCLUDE_DIRS}")
    check_include_file(openssl/ssl.h HAVE_OPENSSL_SSL_H)

    link_directories(${OPENSSL_LIBRARY_DIRS})

  else(OPENSSL_FOUND)
    # message(STATUS "Searching for openssl using FindOpenSSL.cmake script...")
    include(FindOpenSSL)

    if(OPENSSL_CRYPTO_LIBRARIES)
      set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_CRYPTO_LIBRARIES}")
    endif(OPENSSL_CRYPTO_LIBRARIES)

  endif(OPENSSL_FOUND)

  if(OPENSSL_FOUND)
    if(NOT OPENSSL_INCLUDE_DIR)
      set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include"
          CACHE PATH "OpenSSL include dir")
    endif()
    if(NOT OPENSSL_LIB_DIR)
      set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
    endif()
  else(OPENSSL_FOUND)
    set(BUILD_SSL TRUE)
  endif(OPENSSL_FOUND)

  if(OPENSSL_SSL_LIBRARY AND NOT OPENSSL_CRYPTO_LIBRARY)
    string(REPLACE "ssl" "crypto" OPENSSL_CRYPTO_LIBRARY
                   "${OPENSSL_SSL_LIBRARY}")
  endif(OPENSSL_SSL_LIBRARY AND NOT OPENSSL_CRYPTO_LIBRARY)

  set(OPENSSL_LIBRARIES "")

  if(OPENSSL_SSL_LIBRARY)
    list(APPEND OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY})
  endif(OPENSSL_SSL_LIBRARY)

  if(OPENSSL_CRYPTO_LIBRARY)
    list(APPEND OPENSSL_LIBRARIES ${OPENSSL_CRYPTO_LIBRARY})
  endif(OPENSSL_CRYPTO_LIBRARY)
  set(OPENSSL_LIBRARIES "${OPENSSL_LIBRARIES}" CACHE STRING "OpenSSL libraries")

  if(OPENSSL_FOUND)
    message(CHECK_PASS "found")
  else(OPENSSL_FOUND)
    message(CHECK_FAIL "NOT found")
  endif(OPENSSL_FOUND)

  # dump(OPENSSL_CRYPTO_LIBRARY OPENSSL_SSL_LIBRARY OPENSSL_LIBRARY_DIRS OPENSSL_INCLUDE_DIRS OPENSSL_LIBRARIES)

else()
  unset(OPENSSL_LIBRARIES CACHE)
  unset(OPENSSL_CRYPTO_LIBRARY CACHE)
  unset(OPENSSL_SSL_LIBRARY CACHE)
  unset(OPENSSL_INCLUDE_DIR CACHE)
endif()
