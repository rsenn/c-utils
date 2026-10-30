# build_brotli(PIC)
#
# Clones brotli into third_party/brotli and builds it as static libraries
# into ${THIRD_PARTY_PREFIX}. Sets BROTLI_INCLUDE_DIR, BROTLI_LIBRARY_DIR and
# BROTLI_LIBRARIES (full paths of enc, dec, common).
include(${CMAKE_CURRENT_LIST_DIR}/ThirdParty.cmake)

set(BROTLI_GIT_URL "https://github.com/google/brotli.git" CACHE STRING "brotli repository cloned into third_party/brotli")
set(BROTLI_GIT_TAG "v1.2.1" CACHE STRING "brotli tag to clone")

macro(build_brotli PIC)
  third_party_fetch(brotli BROTLI_SOURCE_DIR GIT "${BROTLI_GIT_URL}" TAG "${BROTLI_GIT_TAG}")

  set(BROTLI_LIBRARY_DIR "${THIRD_PARTY_PREFIX}/lib")
  set(BROTLI_INCLUDE_DIR "${THIRD_PARTY_PREFIX}/include")
  set(BROTLI_LIBRARIES)
  foreach(_brotli_lib brotlienc brotlidec brotlicommon)
    list(APPEND BROTLI_LIBRARIES "${BROTLI_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${_brotli_lib}${CMAKE_STATIC_LIBRARY_SUFFIX}")
  endforeach(_brotli_lib)

  list(GET BROTLI_LIBRARIES 0 _first)
  third_party_build(brotli "${BROTLI_SOURCE_DIR}" "${THIRD_PARTY_PREFIX}" ${PIC} "${_first}"
                    CMAKE_ARGS -DBROTLI_DISABLE_TESTS=ON -DBROTLI_BUILD_TOOLS=OFF)
  set(BROTLI_FOUND TRUE)
endmacro(build_brotli)
