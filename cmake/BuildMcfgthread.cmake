# build_mcfgthread()
#
# Clones mcfgthread into third_party/mcfgthread and puts its headers on the
# include path. Header only use, so nothing is built: version.h, which upstream
# generates with meson from version.h.in, is substituted here instead.
# Sets MCFGTHREAD_INCLUDE_DIR.
include(${CMAKE_CURRENT_LIST_DIR}/ThirdParty.cmake)

set(MCFGTHREAD_GIT_URL "https://github.com/lhmouse/mcfgthread.git" CACHE STRING "mcfgthread repository cloned into third_party/mcfgthread")
set(MCFGTHREAD_GIT_TAG "v2.5-alpha" CACHE STRING "mcfgthread tag to clone")

macro(build_mcfgthread)
  third_party_fetch(mcfgthread MCFGTHREAD_SOURCE_DIR GIT "${MCFGTHREAD_GIT_URL}" TAG "${MCFGTHREAD_GIT_TAG}")

  set(MCFGTHREAD_INCLUDE_DIR "${MCFGTHREAD_SOURCE_DIR}")
  set(abi_major 2)
  set(abi_minor 5)
  set(abi_string "2.5.alpha")
  configure_file("${MCFGTHREAD_SOURCE_DIR}/mcfgthread/version.h.in"
                 "${CMAKE_BINARY_DIR}/mcfgthread-generated/mcfgthread/version.h" @ONLY)

  # mcfgthread/fwd.h includes it as a quote-form sibling ("version.h")
  include_directories("${MCFGTHREAD_INCLUDE_DIR}" "${CMAKE_BINARY_DIR}/mcfgthread-generated/mcfgthread")
endmacro(build_mcfgthread)
