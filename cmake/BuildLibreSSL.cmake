# build_libressl(PIC)
#
# Fetches the LibreSSL release into third_party/libressl (the git repository
# needs autogen and an OpenBSD checkout, the release tarball does not) and
# builds libssl, libcrypto and libtls as static libraries into
# ${THIRD_PARTY_PREFIX}. Sets OPENSSL_INCLUDE_DIR, OPENSSL_LIB_DIR,
# OPENSSL_SSL_LIBRARY, OPENSSL_CRYPTO_LIBRARY, OPENSSL_LIBRARIES and
# TLS_LIBRARIES, so the rest of the build treats it like OpenSSL.
include(${CMAKE_CURRENT_LIST_DIR}/ThirdParty.cmake)

set(LIBRESSL_VERSION "4.3.3" CACHE STRING "LibreSSL release to fetch into third_party/libressl")
set(LIBRESSL_URL "https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-${LIBRESSL_VERSION}.tar.gz"
    CACHE STRING "LibreSSL release tarball")

macro(build_libressl PIC)
  third_party_fetch(libressl LIBRESSL_SOURCE_DIR URL "${LIBRESSL_URL}")

  set(OPENSSL_LIB_DIR "${THIRD_PARTY_PREFIX}/lib")
  set(OPENSSL_INCLUDE_DIR "${THIRD_PARTY_PREFIX}/include")
  set(OPENSSL_INCLUDE_DIRS "${OPENSSL_INCLUDE_DIR}")
  set(OPENSSL_LIBRARY_DIRS "${OPENSSL_LIB_DIR}")
  set(OPENSSL_SSL_LIBRARY "${OPENSSL_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}")

  third_party_build(libressl "${LIBRESSL_SOURCE_DIR}" "${THIRD_PARTY_PREFIX}" ${PIC} "${OPENSSL_SSL_LIBRARY}"
                    CMAKE_ARGS -DLIBRESSL_APPS=OFF -DLIBRESSL_TESTS=OFF)

  set(OPENSSL_LIBRARIES "${OPENSSL_SSL_LIBRARY};${OPENSSL_CRYPTO_LIBRARY}")
  set(TLS_LIBRARIES "${OPENSSL_LIBRARIES}")
  set(OPENSSL_FOUND TRUE)
endmacro(build_libressl)
