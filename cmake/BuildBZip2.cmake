# build_bzip2(PIC)
#
# Clones bzip2 into third_party/bzip2 and builds libbz2 as a static library
# into ${THIRD_PARTY_PREFIX} (bzip2 has no cmake build of its own, so
# cmake/overlay/bzip2.cmake is used). Sets BZIP2_INCLUDE_DIR, BZIP2_LIBRARY_DIR
# and BZIP2_LIBRARY (full path).
include(${CMAKE_CURRENT_LIST_DIR}/ThirdParty.cmake)

set(BZIP2_GIT_URL "https://github.com/libarchive/bzip2.git" CACHE STRING "bzip2 repository cloned into third_party/bzip2")
set(BZIP2_GIT_TAG "bzip2-1.0.8" CACHE STRING "bzip2 tag to clone")

macro(build_bzip2 PIC)
  third_party_fetch(bzip2 BZIP2_SOURCE_DIR GIT "${BZIP2_GIT_URL}" TAG "${BZIP2_GIT_TAG}"
                    OVERLAY "${CMAKE_CURRENT_LIST_DIR}/overlay/bzip2.cmake")

  set(BZIP2_LIBRARY_DIR "${THIRD_PARTY_PREFIX}/lib")
  set(BZIP2_INCLUDE_DIR "${THIRD_PARTY_PREFIX}/include")
  set(BZIP2_LIBRARY "${BZIP2_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}bz2${CMAKE_STATIC_LIBRARY_SUFFIX}")

  third_party_build(bzip2 "${BZIP2_SOURCE_DIR}" "${THIRD_PARTY_PREFIX}" ${PIC} "${BZIP2_LIBRARY}")
  set(BZIP2_LIBRARIES "${BZIP2_LIBRARY}")
  set(BZIP2_FOUND TRUE)
endmacro(build_bzip2)
