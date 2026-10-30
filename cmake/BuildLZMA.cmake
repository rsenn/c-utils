# build_lzma(PIC)
#
# Clones xz into third_party/xz and builds liblzma as a static library into
# ${THIRD_PARTY_PREFIX}. Sets LIBLZMA_INCLUDE_DIR, LIBLZMA_LIBRARY_DIR and
# LIBLZMA_LIBRARY (full path).
include(${CMAKE_CURRENT_LIST_DIR}/ThirdParty.cmake)

set(LZMA_GIT_URL "https://github.com/tukaani-project/xz.git" CACHE STRING "xz repository cloned into third_party/xz")
set(LZMA_GIT_TAG "v5.8.2" CACHE STRING "xz tag to clone")

macro(build_lzma PIC)
  third_party_fetch(xz LZMA_SOURCE_DIR GIT "${LZMA_GIT_URL}" TAG "${LZMA_GIT_TAG}")

  set(LIBLZMA_LIBRARY_DIR "${THIRD_PARTY_PREFIX}/lib")
  set(LIBLZMA_INCLUDE_DIR "${THIRD_PARTY_PREFIX}/include")
  set(LIBLZMA_LIBRARY "${LIBLZMA_LIBRARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}lzma${CMAKE_STATIC_LIBRARY_SUFFIX}")

  third_party_build(xz "${LZMA_SOURCE_DIR}" "${THIRD_PARTY_PREFIX}" ${PIC} "${LIBLZMA_LIBRARY}"
                    CMAKE_ARGS -DXZ_TOOL_XZ=OFF -DXZ_TOOL_XZDEC=OFF -DXZ_TOOL_LZMADEC=OFF -DXZ_TOOL_LZMAINFO=OFF
                               -DXZ_TOOL_SYMLINKS=OFF -DXZ_TOOL_SCRIPTS=OFF -DXZ_DOC=OFF -DXZ_NLS=OFF
                               -DXZ_MICROLZMA_DECODER=OFF -DXZ_MICROLZMA_ENCODER=OFF -DXZ_LZIP_DECODER=OFF)
  set(LIBLZMA_LIBRARIES "${LIBLZMA_LIBRARY}")
  set(LIBLZMA_FOUND TRUE)
endmacro(build_lzma)
