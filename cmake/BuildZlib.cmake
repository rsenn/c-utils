# build_zlib(PIC)
#
# Clones zlib into third_party/zlib and builds it as a static library into
# ${THIRD_PARTY_PREFIX}. Sets ZLIB_INCLUDE_DIR, ZLIB_LIBRARY_DIR and
# ZLIB_LIBRARY (full path).
include(${CMAKE_CURRENT_LIST_DIR}/ThirdParty.cmake)

set(ZLIB_GIT_URL "https://github.com/madler/zlib.git" CACHE STRING "zlib repository cloned into third_party/zlib")
set(ZLIB_GIT_TAG "v1.3.1" CACHE STRING "zlib tag to clone")

macro(build_zlib PIC)
  third_party_fetch(zlib ZLIB_SOURCE_DIR GIT "${ZLIB_GIT_URL}" TAG "${ZLIB_GIT_TAG}")

  set(_name z)
  if(CMAKE_SYSTEM_NAME STREQUAL "WASI")
    set(_name zlibstatic)
  endif(CMAKE_SYSTEM_NAME STREQUAL "WASI")

  set(ZLIB_LIBRARY_DIR "${THIRD_PARTY_PREFIX}/lib")
  set(ZLIB_INCLUDE_DIR "${THIRD_PARTY_PREFIX}/include")
  set(ZLIB_LIBRARY "${ZLIB_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${_name}${CMAKE_STATIC_LIBRARY_SUFFIX}")

  third_party_build(zlib "${ZLIB_SOURCE_DIR}" "${THIRD_PARTY_PREFIX}" ${PIC} "${ZLIB_LIBRARY}"
                    CMAKE_ARGS -DZLIB_BUILD_EXAMPLES=OFF)
  set(ZLIB_LIBRARIES "${ZLIB_LIBRARY}")
  set(ZLIB_FOUND TRUE)
endmacro(build_zlib)
