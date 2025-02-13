if(BUILD_SSL)
  add_definitions(-DOPENSSL_THREADS=1)
  if(CMAKE_C_COMPILER MATCHES "-musl")
    set(MUSL TRUE)
  endif(CMAKE_C_COMPILER MATCHES "-musl")
  if(MUSL)
    add_definitions(-DOPENSSL_NO_SECURE_MEMORY=1)
  endif(MUSL)
  #[[if(UNIX OR LINUX)
    add_definitions(-DOPENSSL_SYS_UNIX=1)
  endif(UNIX OR LINUX)]]
  if(DIET OR MUSL)
    set(CHK_DLFCN FALSE)
    set(DSO_NONE ON)
  endif(DIET OR MUSL)
  set(WITH_APPS FALSE)
  set(OPENSSL_NO_INSTALL TRUE)
  set(OPENSSL_VERSION "1.1.1l")
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/openssl-${OPENSSL_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
      file(DOWNLOAD "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz" "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}.tar.gz")
    file(
      WRITE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1l-ecerr.patch"
      "diff -ru openssl-1.1.1l/crypto/ec/ec_ameth.c openssl/crypto/ec/ec_ameth.c\n--- openssl-1.1.1l/crypto/ec/ec_ameth.c\t2021-08-24 15:38:47.000000000 +0200\n+++ openssl/crypto/ec/ec_ameth.c\t2021-09-04 03:56:27.055271631 +0200\n@@ -7,6 +7,7 @@\n  * https://www.openssl.org/source/license.html\n  */\n \n+#include <openssl/ecerr.h>\n #include <stdio.h>\n #include \"internal/cryptlib.h\"\n #include <openssl/x509.h>\n@@ -18,6 +19,10 @@\n #include \"crypto/evp.h\"\n #include \"ec_local.h\"\n \n+#ifndef EC_R_MISSING_OID\n+#define EC_R_MISSING_OID 167\n+#endif\n+\n #ifndef OPENSSL_NO_CMS\n static int ecdh_cms_decrypt(CMS_RecipientInfo *ri);\n static int ecdh_cms_encrypt(CMS_RecipientInfo *ri);\ndiff -ru openssl-1.1.1l/crypto/ec/ec_asn1.c openssl/crypto/ec/ec_asn1.c\n--- openssl-1.1.1l/crypto/ec/ec_asn1.c\t2021-08-24 15:38:47.000000000 +0200\n+++ openssl/crypto/ec/ec_asn1.c\t2021-09-04 03:57:06.415269417 +0200\n@@ -14,6 +14,10 @@\n #include <openssl/objects.h>\n #include \"internal/nelem.h\"\n \n+#ifndef EC_R_MISSING_OID\n+#define EC_R_MISSING_OID 167\n+#endif\n+\n int EC_GROUP_get_basis_type(const EC_GROUP *group)\n {\n     int i;\ndiff -ru openssl-1.1.1l/crypto/ec/ec_err.c openssl/crypto/ec/ec_err.c\n--- openssl-1.1.1l/crypto/ec/ec_err.c\t2021-08-24 15:38:47.000000000 +0200\n+++ openssl/crypto/ec/ec_err.c\t2021-09-04 03:57:44.175267292 +0200\n@@ -11,6 +11,10 @@\n #include <openssl/err.h>\n #include <openssl/ecerr.h>\n \n+#ifndef EC_R_MISSING_OID\n+#define EC_R_MISSING_OID 167\n+#endif\n+\n #ifndef OPENSSL_NO_ERR\n \n static const ERR_STRING_DATA EC_str_functs[] = {\n"
    )
    file(
      WRITE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1h-getrandom.patch"
      "--- 3rdparty/openssl-1.1.1h/crypto/rand/rand_unix.c\t2020-09-22 14:55:07.000000000 +0200\n+++ 3rdparty/openssl/crypto/rand/rand_unix.c\t2020-11-26 06:54:18.694873358 +0100\n@@ -19,6 +19,9 @@\n #include \"crypto/rand.h\"\n #include <stdio.h>\n #include \"internal/dso.h\"\n+#ifdef __dietlibc__\n+# include <sys/random.h>\n+#endif\n #ifdef __linux\n # include <sys/syscall.h>\n # ifdef DEVRANDOM_WAIT\n@@ -389,7 +392,9 @@\n #  endif\n \n     /* Linux supports this since version 3.17 */\n-#  if defined(__linux) && defined(__NR_getrandom)\n+#  if defined(__dietlibc__)\n+    return getrandom(buf, buflen, 0);\n+#  elif defined(__linux) && defined(__NR_getrandom)\n     return syscall(__NR_getrandom, buf, buflen, 0);\n #  elif (defined(__FreeBSD__) || defined(__NetBSD__)) && defined(KERN_ARND)\n     return sysctl_random(buf, buflen);\n"
    )
    execute_process(COMMAND patch -p0 INPUT_FILE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1l-ecerr.patch" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rdparty")
    #execute_process(COMMAND patch -p0 INPUT_FILE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1h-getrandom.patch" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    #  execute_process(COMMAND patch -p0 INPUT_FILE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1h.diff" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
    # exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty/openssl" -xvzf "${CMAKE_SOURCE_DIR}/3rdparty/openssl-cmake.tar.gz")

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}.tar.gz")

    include("${CMAKE_SOURCE_DIR}/3rdparty/openssl.cmake")

  endif()

  add_definitions(-DOPENSSL_NO_UI_CONSOLE=1)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_NO_UI_CONSOLE=1")

  message(STATUS "Configuring OpenSSL...")
  #add_subdirectory(3rdparty/openssl)

  # if(NOT OPENSSL_LIBRARIES) message(WARNING "Variable OPENSSL_LIBRARIES not set!") endif(NOT OPENSSL_LIBRARIES)

  set(OPENSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/openssl/include" CACHE FILEPATH "")
  include_directories(${OPENSSL_INCLUDE_DIR})
  include_directories(${CMAKE_BINARY_DIR}/3rdparty/openssl/include)

  # set(OPENSSL_SSL_LIBRARY "${CMAKE_BINARY_DIR}/3rdparty/openssl/ssl/libssl.a" CACHE FILEPATH "SSL library") set(OPENSSL_CRYPTO_LIBRARY "${CMAKE_BINARY_DIR}/3rdparty/openssl/crypto/libcrypto.a" CACHE  FILEPATH "Cryptography library")
  set(OPENSSL_SSL_LIBRARY libssl CACHE STRING "SSL library")
  set(OPENSSL_CRYPTO_LIBRARY libcrypto CACHE FILEPATH "Cryptography library")

endif()

if(USE_SSL)
  add_definitions(-DHAVE_OPENSSL=1 -DOPENSSL_API_COMPAT=0x10200000L)
  if(OPENSSL_INCLUDE_DIR)
    include_directories(${CMAKE_BINARY_DIR}/3rdparty/openssl/include ${OPENSSL_INCLUDE_DIR})
  endif()

  if(NOT OPENSSL_LIBRARIES)
    if(OPENSSL_CRYPTO_LIBRARY OR OPENSSL_SSL_LIBRARY)
      set(OPENSSL_LIBARRIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
    endif(OPENSSL_CRYPTO_LIBRARY OR OPENSSL_SSL_LIBRARY)
  endif(NOT OPENSSL_LIBRARIES)

  set(TLS_LIBRARY_DIR ${CMAKE_BINARY_DIR})
  link_directories(${TLS_LIBRARY_DIR})

  set(TLS_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
endif(USE_SSL)
