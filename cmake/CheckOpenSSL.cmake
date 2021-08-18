# ##############################################################################################################################################################################################################################################################################################################################################################################################################
# Check OpenSSL
# ##############################################################################################################################################################################################################################################################################################################################################################################################################
if(USE_SSL AND NOT BUILD_SSL)
  message(STATUS "Using OpenSSL encryption")
  if(NOT OPENSSL_ROOT_DIR)
    if("$ENV{OPENSSL_ROOT}")
      set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT}" CACHE FILEPATH "OpenSSL root directory")
      # else("$ENV{OPENSSL_ROOT}") set(OPENSSL_ROOT_DIR "${CMAKE_INSTALL_PREFIX}" CACHE FILEPATH "OpenSSL root directory")
    endif("$ENV{OPENSSL_ROOT}")
  endif(NOT OPENSSL_ROOT_DIR)
  set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
  # unset(OPENSSL_FOUND CACHE)
  message(STATUS "Searching pkgconfig database for openssl...")
  pkg_search_module(OPENSSL openssl libssl)

  message(STATUS "pkg_search_module(OPENSSL openssl)")

  if(OPENSSL_FOUND)
    message(STATUS "Found openssl at ${OPENSSL_LIBRARY_DIRS} ${OPENSSL_INCLUDE_DIRS} ...")

    if(pkgcfg_lib_OPENSSL_crypto)
      set(OPENSSL_CRYPTO_LIBRARY "${pkgcfg_lib_OPENSSL_crypto}" CACHE FILEPATH "OpenSSL crypto library")
    else(pkgcfg_lib_OPENSSL_crypto)
      set(OPENSSL_CRYPTO_LIBRARY "crypto" CACHE FILEPATH "OpenSSL crypto library")
    endif(pkgcfg_lib_OPENSSL_crypto)

    if(pkgcfg_lib_OPENSSL_ssl)
      set(OPENSSL_SSL_LIBRARY "${pkgcfg_lib_OPENSSL_ssl}" CACHE FILEPATH "OpenSSL ssl library")
    else(pkgcfg_lib_OPENSSL_ssl)
      set(OPENSSL_SSL_LIBRARY "ssl" CACHE FILEPATH "OpenSSL ssl library")
    endif(pkgcfg_lib_OPENSSL_ssl)

    if(OPENSSL_CRYPTO_LIBRARY AND OPENSSL_SSL_LIBRARY)
      set(TLS_LIBRARIES "${OPENSSL_SSL_LIBRARY};${OPENSSL_CRYPTO_LIBRARY}" CACHE STRING "TLS libraries")
    endif(OPENSSL_CRYPTO_LIBRARY AND OPENSSL_SSL_LIBRARY)

    list(APPEND CMAKE_REQUIRED_INCLUDES "${OPENSSL_INCLUDE_DIRS}")
    check_include_file(openssl/ssl.h HAVE_OPENSSL_SSL_H)

    link_directories(${OPENSSL_LIBRARY_DIRS})

  else(OPENSSL_FOUND)
    message(STATUS "Searching for openssl using FindOpenSSL.cmake script...")
    include(FindOpenSSL)

    if(OPENSSL_CRYPTO_LIBRARIES)
      set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_CRYPTO_LIBRARIES}")
    endif(OPENSSL_CRYPTO_LIBRARIES)

    # dump(OPENSSL_FOUND OPENSSL_INCLUDE_DIR OPENSSL_CRYPTO_LIBRARY OPENSSL_CRYPTO_LIBRARIES OPENSSL_SSL_LIBRARY OPENSSL_SSL_LIBRARIES OPENSSL_LIBRARIES OPENSSL_VERSION)
  endif(OPENSSL_FOUND)

  # if(NOT HAVE_OPENSSL_SSL_H) set(OPENSSL_FOUND FALSE) endif(NOT HAVE_OPENSSL_SSL_H) dump(OPENSSL_FOUND OPENSSL_ROOT OPENSSL_PREFIX OPENSSL_SSL_LIBRARY OPENSSL_CRYPTO_LIBRARY)
  dump(OPENSSL_CRYPTO_LIBRARY OPENSSL_SSL_LIBRARY OPENSSL_LIBRARY_DIRS OPENSSL_INCLUDE_DIRS)

  if(OPENSSL_FOUND)
    if(NOT OPENSSL_INCLUDE_DIR)
      set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    endif()
    if(NOT OPENSSL_LIB_DIR)
      set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
    endif()
  else(OPENSSL_FOUND)
    set(BUILD_SSL TRUE)
  endif(OPENSSL_FOUND)

  # message(STATUS "OPENSSL_FOUND=${OPENSSL_FOUND}\n   OPENSSL_INCLUDE_DIR=${OPENSSL_LIB_DIR}\n OPENSSL_LIB_DIR=${OPENSSL_INCLUDE_DIR}\n   OPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}\n OPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}\n OPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}")

  if(OPENSSL_SSL_LIBRARY AND NOT OPENSSL_CRYPTO_LIBRARY)
    string(REPLACE "libssl" "libcrypto" OPENSSL_CRYPTO_LIBRARY "${OPENSSL_SSL_LIBRARY}")
  endif(OPENSSL_SSL_LIBRARY AND NOT OPENSSL_CRYPTO_LIBRARY)

else()
  unset(OPENSSL_LIBRARIES CACHE)
  unset(OPENSSL_CRYPTO_LIBRARY CACHE)
  unset(OPENSSL_SSL_LIBRARY CACHE)
  unset(OPENSSL_INCLUDE_DIR CACHE)
endif()
# link_libraries( ${ZLIB_LIBRARY} ${BZIP2_LIBRARY} ${LIBLZMA_LIBRARY} ${TLS_LIBRARIES} ) add_definitions(-Dinline=__inline)
if(NOT OPENSSL_SSL_LIBRARY OR OPENSSL_SSL_LIBRARY STREQUAL "")

  # message("OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY}")
  string(REPLACE "libcrypto" "libssl" OPENSSL_SSL_LIBRARY "${OPENSSL_CRYPTO_LIBRARY}")
  # message("OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")
endif(NOT OPENSSL_SSL_LIBRARY OR OPENSSL_SSL_LIBRARY STREQUAL "")

if(OPENSSL_FOUND)
  # dump(pkgcfg_lib_OPENSSL_ssl pkgcfg_lib_OPENSSL_crypto)

  if(pkgcfg_lib_OPENSSL_crypto)
    # if(NOT pkgcfg_lib_OPENSSL_ssl OR "${pkgcfg_lib_OPENSSL_ssl}" STREQUAL "")
    string(REPLACE "libcrypto" "libssl" pkgcfg_lib_OPENSSL_ssl "${pkgcfg_lib_OPENSSL_crypto}")
    # endif(NOT pkgcfg_lib_OPENSSL_ssl OR "${pkgcfg_lib_OPENSSL_ssl}" STREQUAL "")
  endif(pkgcfg_lib_OPENSSL_crypto)
  if(pkgcfg_lib_OPENSSL_ssl)
    set(OPENSSL_SSL_LIBRARY "${pkgcfg_lib_OPENSSL_ssl}")
  else(pkgcfg_lib_OPENSSL_ssl)
    string(REPLACE "libcrypto" "libssl" OPENSSL_SSL_LIB "${OPENSSL_CRYPTO_LIBRARY}")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_SSL_LIB}" CACHE STRING "SSL library")
  endif(pkgcfg_lib_OPENSSL_ssl)

  if(OPENSSL_SSL_LIBRARY STREQUAL "" OR NOT OPENSSL_SSL_LIBRARY)
    string(REPLACE "libcrypto" "libssl" OPENSSL_SSL_LIB "${OPENSSL_CRYPTO_LIBRARY}")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_SSL_LIB}" CACHE FILEPATH "SSL library")
  endif(OPENSSL_SSL_LIBRARY STREQUAL "" OR NOT OPENSSL_SSL_LIBRARY)

  if(OPENSSL_CRYPTO_LIBRARY STREQUAL "" OR NOT OPENSSL_CRYPTO_LIBRARY)
    string(REPLACE "libssl" "libcrypto" OPENSSL_CRYPTO_LIB "${OPENSSL_SSL_LIBRARY}")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_CRYPTO_LIB}" CACHE FILEPATH "SSL library")
  endif(OPENSSL_CRYPTO_LIBRARY STREQUAL "" OR NOT OPENSSL_CRYPTO_LIBRARY)

  if(OPENSSL_SSL_LIBRARY)
    list(APPEND OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY})
  endif(OPENSSL_SSL_LIBRARY)

  if(OPENSSL_CRYPTO_LIBRARY)
    list(APPEND OPENSSL_LIBRARIES ${OPENSSL_CRYPTO_LIBRARY})
  endif(OPENSSL_CRYPTO_LIBRARY)
endif(OPENSSL_FOUND)
