set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_WARN_VS8 OFF)

project(dirlist C)

option(USE_LZMA "lzma compression" ON)
option(USE_BZIP "bzip2 compression" ON)
option(USE_ZLIB "zlib compression" ON)
option(USE_SSL "ssl encryption" ON)

option(USE_SELECT "Use select() syscall" ON)
option(USE_POLL "Use poll() syscall" OFF)
option(USE_EPOLL  "Use epoll() syscall" OFF)


set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)
set(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL)


if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl" OR NOT EXISTS "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
    file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
    file(DOWNLOAD "http://distfiles.gentoo.org/distfiles/openssl-1.1.0j.tar.gz" "${CMAKE_SOURCE_DIR}/openssl.tar.gz" SHOW_PROGRESS)
  endif()

  exec_program(bsdtar ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
  file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.0j" "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
  file(READ "${CMAKE_SOURCE_DIR}/build/cmake/BuildOpenSSL.cmake" OPENSSL_CMAKELISTS)
  file(WRITE "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt" "${OPENSSL_CMAKELISTS}")
  exec_program(patch "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl" ARGS
    -p0 -i ../../build/cmake/openssl-1.1.0j-pkgs7.patch
  )


endif()
#if(EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
#  add_subdirectory(3rdparty/openssl)
#endif()

set(BUILD_SHARED_LIBS FALSE)

if(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS FALSE)
else(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS TRUE)
endif(NOT ENABLE_SHARED)

include(FindPkgConfig)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFile)

function(DUMP VAR)
  message("\n\nVariable dump of: " ${ARGV} "\n")

  foreach(VAR ${ARGV})
  message("\t${VAR} = ${${VAR}}")
  endforeach(VAR ${ARGV})
  message("\n")
endfunction(DUMP VAR)
string(REGEX REPLACE ".*/" "" COMPILER_NAME "${CMAKE_C_COMPILER}")

if(COMPILER_NAME MATCHES "em.*")
  set(EMSCRIPTEN TRUE)
  DUMP(EMSCRIPTEN EMCC_SUFFIX EMSCRIPTEN_PREFIX EMSCRIPTEN_TOOLCHAIN_PATH COMPILER_NAME  CMAKE_EXE_LINKER_FLAGS)
endif(COMPILER_NAME MATCHES "em.*")


check_function_exists(lseek64 HAVE_LSEEK64)
if(NOT HAVE_LSEEK64)
  check_function_exists(llseek HAVE_LLSEEK)
  if(NOT HAVE_LLSEEK)
    check_function_exists(lseek HAVE_LSEEK)
    if(NOT HAVE_LSEEK)
    else(NOT HAVE_LSEEK)
      add_definitions(-Dio_seek=lseek)
    endif(NOT HAVE_LSEEK)
  else(NOT HAVE_LLSEEK)
    add_definitions(-Dio_seek=llseek)
  endif(NOT HAVE_LLSEEK)
else(NOT HAVE_LSEEK64)
    add_definitions(-Dio_seek=lseek64)
endif(NOT HAVE_LSEEK64)

if(USE_POLL STREQUAL "OFF" AND NOT USE_SELECT STREQUAL "ON")
  message(INFO "Checking for poll()...")
  check_function_exists(poll_wait HAVE_POLL)
  if(HAVE_POLL)
    message(INFO "Checking for poll()... OK")
    set(USE_POLL TRUE)
    set(USE_SELECT FALSE)
  else(HAVE_POLL)
    message(INFO "Checking for poll()... NOT FOUND")
    set(USE_POLL TRUE)
  endif(HAVE_POLL)
endif(USE_POLL STREQUAL "OFF")

message("USE_SELECT=${USE_SELECT}")
message("USE_POLL=${USE_POLL}")
message("USE_EPOLL=${USE_EPOLL}")
if(USE_POLL STREQUAL "OFF" AND USE_SELECT STREQUAL "OFF" AND USE_EPOLL STREQUAL "OFF")
  set(USE_SELECT TRUE)
endif()
if(USE_POLL STREQUAL "ON")
  add_definitions(-DHAVE_POLL=1 -DIOPAUSE_POLL=1)
endif()

if(USE_SELECT STREQUAL "ON")
  add_definitions(-DUSE_SELECT=1 -DIOPAUSE_SELECT=1)
endif()


#message(INFO "Checking for epoll()...")
#check_function_exists(epoll_wait HAVE_EPOLL)
#if(HAVE_EPOLL)
#  message(INFO "Checking for epoll()... OK")
#  set(USE_EPOLL ON)
#  set(USE_SELECT OFF)
#else(HAVE_EPOLL)
#  message(INFO "Checking for epoll()... NOT FOUND")
#  set(USE_EPOLL OFF)
#endif(HAVE_EPOLL)

if(USE_EPOLL STREQUAL ON)
  add_definitions(-DHAVE_EPOLL=1)
endif()

check_library_exists(compat syscall "" HAVE_COMPAT_LIB)
if(HAVE_COMPAT_LIB)
  set(LIBCOMPAT compat)
endif()

check_library_exists(m roundf "" HAVE_MATH_LIB)
if(HAVE_MATH_LIB)
  set(LIBMATH m)
endif()

#include(${CMAKE_SOURCE_DIR}/FindZlib.cmake)
#include(${CMAKE_SOURCE_DIR}/FindBZip2.cmake)
#include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)

include(${CMAKE_SOURCE_DIR}/build/cmake/CheckInline.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG=1 -DDEBUG_OUTPUT=1)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_MINSIZEREL)
foreach(CompilerFlag ${CompilerFlags})
  unset(FLAG)
  string(REPLACE "/MD" "/MT" FLAG "${${CompilerFlag}}")
  #string(REPLACE "/MTd" "/MT" FLAG "${FLAG}")
  if(${CompilerFlag} MATCHES "DEBUG")
    set(FLAG "${FLAG} -DDEBUG=1")
  endif()
  string(REPLACE " " ";" FLAG "${FLAG}")
  list(REMOVE_DUPLICATES FLAG)
  string(REPLACE ";" " " FLAG "${FLAG}")
  unset(${CompilerFlag} CACHE)
    set(${CompilerFlag} "${FLAG}" CACHE STRING "Compiler flags ${CompilerFlag}")
  message("${CompilerFlag}: ${${CompilerFlag}}")
endforeach()

include(${CMAKE_SOURCE_DIR}/build/cmake/Werror.cmake)


if(USE_LZMA)
  ################################################################################
  ## Check LibLZMA
  ################################################################################
  #pkg_search_module(LIBLZMA liblzma lzma)
  #if(LIBLZMA_FOUND)
  #  if(pkgcfg_lib_LIBLZMA_lzma)
  #    set(LIBLZMA_LIBRARY ${pkgcfg_lib_LIBLZMA_lzma} CACHE FILEPATH "")
  #  endif(pkgcfg_lib_LIBLZMA_lzma)
  #endif(LIBLZMA_FOUND)
  #
  #if(NOT LIBLZMA_FOUND)
  #  include(FindLibLZMA)
  #endif(NOT LIBLZMA_FOUND)

  if(NOT LIBLZMA_FOUND)
    check_library_exists(lzma lzma_auto_decoder "" HAVE_LIBLZMA)
    check_include_file(lzma.h HAVE_LZMA_H)

    if(HAVE_LIBLZMA AND HAVE_LZMA_H)
      set(LIBLZMA_FOUND TRUE)
      set(LIBLZMA_INCLUDE_DIR "")
      set(LIBLZMA_LIBRARY lzma)
      message(STATUS "Found liblzma")
      message(STATUS "  LIBLZMA_LIBRARY=${LIBLZMA_LIBRARY}")
    endif(HAVE_LIBLZMA AND HAVE_LZMA_H)

  endif(NOT LIBLZMA_FOUND)

else(USE_LZMA)
      unset(LIBLZMA_LIBRARY CACHE)
      unset(LIBLZMA_INCLUDE_DIR CACHE)
      unset(LIBLZMA_FOUND CACHE)

endif(USE_LZMA)


if(USE_ZLIB)
  ################################################################################
  ## Check Zlib
  ################################################################################
  pkg_search_module(ZLIB zlib libz z)
  if(ZLIB_FOUND)
    if(pkgcfg_lib_ZLIB_z)
      set(ZLIB_LIBRARY_RELEASE "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
      set(ZLIB_LIBRARY_DEBUG "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
    endif(pkgcfg_lib_ZLIB_z)
  else(ZLIB_FOUND)
    #include(FindZlib)
  endif(ZLIB_FOUND)

  if(ZLIB_LIBRARY_RELEASE)
      set(ZLIB_LIBRARY "${ZLIB_LIBRARY_RELEASE}" CACHE FILEPATH "zlib library")
  endif(ZLIB_LIBRARY_RELEASE)

else(USE_ZLIB)
unset(ZLIB_LIBRARY_RELEASE CACHE)
unset(ZLIB_LIBRARY_DEBUG CACHE)
unset(ZLIB_LIBRARY CACHE)
endif(USE_ZLIB)


  if(USE_BZIP)
  ################################################################################
  ## Check BZip2
  ################################################################################
  pkg_search_module(BZIP2 libbz2 bzip2)
  if(NOT BZIP2_FOUND)
    include(FindBZip2)
  endif(NOT BZIP2_FOUND)
endif(USE_BZIP)

################################################################################
## Check OpenSSL
################################################################################
if(USE_SSL)
  if(NOT OPENSSL_ROOT_DIR)
    set(OPENSSL_ROOT_DIR "${CMAKE_INSTALL_PREFIX}" CACHE FILEPATH "OpenSSL root directory")
  endif()

  pkg_search_module(OPENSSL openssl libssl)
  if(OPENSSL_FOUND)
    if(pkgcfg_lib_OPENSSL_crypto)
    set(OPENSSL_CRYPTO_LIBRARY "${pkgcfg_lib_OPENSSL_crypto}")
      list(APPEND OPENSSL_LIBRARIES ${pkgcfg_lib_OPENSSL_crypto})
    endif(pkgcfg_lib_OPENSSL_crypto)
    if(pkgcfg_lib_OPENSSL_ssl)
      set(OPENSSL_SSL_LIBRARY "${pkgcfg_lib_OPENSSL_ssl}")

      list(APPEND OPENSSL_LIBRARIES ${pkgcfg_lib_OPENSSL_ssl})
    endif(pkgcfg_lib_OPENSSL_ssl)
  else(OPENSSL_FOUND)
    include(FindOpenSSL)
  endif(OPENSSL_FOUND)


  if(OPENSSL_FOUND)
    if(NOT OPENSSL_INCLUDE_DIR)
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    endif()

     if(NOT OPENSSL_LIB_DIR)
    set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
    endif()

    add_definitions(-DHAVE_OPENSSL=1)
    if(OPENSSL_INCLUDE_DIR)
      include_directories(${OPENSSL_INCLUDE_DIR})
    endif()
    if(OPENSSL_CRYPTO_LIBRARY OR OPENSSL_SSL_LIBRARY)
      set(OPENSSL_LIBARRIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
      #link_libraries(${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
    endif()

  endif(OPENSSL_FOUND)
  message(STATUS "OPENSSL_FOUND=${OPENSSL_FOUND}\nOPENSSL_INCLUDE_DIR=${OPENSSL_LIB_DIR}\nOPENSSL_LIB_DIR=${OPENSSL_INCLUDE_DIR}\nOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}\nOPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY} OPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}")
else(USE_SSL)
 unset(OPENSSL_LIBRARIES CACHE)
 unset(OPENSSL_CRYPTO_LIBRARY CACHE)
 unset(OPENSSL_SSL_LIBRARY CACHE)
 unset(OPENSSL_INCLUDE_DIR CACHE)
endif(USE_SSL)
#link_libraries(
#  ${ZLIB_LIBRARY}
#  ${BZIP2_LIBRARY}
#  ${LIBLZMA_LIBRARY}
#  ${CRYPTO_LIBRARIES}
#  )
add_definitions(-Dinline=__inline)



if(MINGW)
  set(LIBMSWSOCK ws2_32)
elseif(WIN32 OR WIN64 OR MSVC)
  set(LIBMSWSOCK wsock32)
endif()

if(USE_SELECT)
   add_definitions(-DUSE_SELECT=1)
endif(USE_SELECT)

if(WIN32 OR MSVC)
  if(USE_WIDECHAR)
     add_definitions(-DUSE_WIDECHAR=1)
  endif(USE_WIDECHAR)
endif(WIN32 OR MSVC)

if(NOT ZLIB_FOUND OR NOT EXISTS "${ZLIB_LIBRARY}")
  unset(ZLIB_FOUND CACHE)
  unset(ZLIB_LIBRARY CACHE)
endif()

if(NOT ZLIB_LIBRARY)
  message("Building zlib from 3rdparty/zlib")
  file(GLOB ZLIB_SOURCES 3rdparty/zlib/*.c)
  add_library(z ${ZLIB_SOURCES})
  set(ZLIB_LIBRARY_DEBUG z CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY_RELEASE z CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY z CACHE FILEPATH "" FORCE)
  set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/zlib CACHE PATH "" FORCE)
endif(NOT ZLIB_LIBRARY)

if(NOT BZIP2_LIBRARY)
  message("Building libbz2 from 3rdparty/bzip2")
  file(GLOB LIBBZ2_SOURCES 3rdparty/bzip2/*.c)
  add_library(bz2 ${LIBBZ2_SOURCES})
  set(BZIP2_LIBRARY_DEBUG bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY_RELEASE bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBR+ARY bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/bzip2 CACHE PATH "" FORCE)
endif(NOT BZIP2_LIBRARY)
#
#if(NOT LIBLZMA_LIBRARY)
#  message("Building liblzma from 3rdparty/xz")
#  file(GLOB LIBLZMA_SOURCES 3rdparty/xz/*/*.c 3rdparty/xz/*/*/*.c)
#  add_library(lzma ${LIBLZMA_SOURCES})
#  set_target_properties(lzma PROPERTIES
#    COMPILE_DEFINITIONS "HAVE_CONFIG_H=1"
#  )
#  set(LIBLZMA_LIBRARY lzma CACHE FILEPATH "" FORCE)
#  set(LIBLZMA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/xz/liblzma/api CACHE PATH "" FORCE)
#endif(NOT LIBLZMA_LIBRARY)

message("ZLIB_LIBRARY: ${ZLIB_LIBRARY} ZLIB_FOUND: ${ZLIB_FOUND}")
if(ZLIB_LIBRARY)
 # link_libraries(${ZLIB_LIBRARY})
  include_directories(${ZLIB_INCLUDE_DIR})
  add_definitions(-DHAVE_ZLIB=1)
else(ZLIB_LIBRARY)
  add_definitions(-DHAVE_ZLIB=0)
endif(ZLIB_LIBRARY)

message("BZIP2_LIBRARY: ${BZIP2_LIBRARY} BZIP2_FOUND: ${BZIP2_FOUND}")
if(BZIP2_LIBRARY)
  add_definitions(-DHAVE_LIBBZ2=1)
  include_directories(${BZIP2_INCLUDE_DIR})
else(BZIP2_LIBRARY)
  add_definitions(-DHAVE_LIBBZ2=0)
endif(BZIP2_LIBRARY)

message("LIBLZMA_LIBRARY: ${LIBLZMA_LIBRARY} LIBLZMA_FOUND: ${LIBLZMA_FOUND}")
if(LIBLZMA_LIBRARY)
  #link_libraries(${LIBLZMA_LIBRARY})
  include_directories(${LIBLZMA_INCLUDE_DIR})
  add_definitions(-DHAVE_LIBLZMA=1 -DLZMA_API_STATIC=1)
else(LIBLZMA_LIBRARY)
  add_definitions(-DHAVE_LIBLZMA=0)
endif(LIBLZMA_LIBRARY)

link_directories(AFTER "${CMAKE_BINARY_DIR}")

set(MODSUBDIRS alloc array binfmt buffer byte case cb cbmap charbuf coff dir dns elf env errmsg expand fmt gpio hashmap hmap http iarray ihex io json list map mmap ndelay omf open path pe playlist process range rdir safemult scan
  #sig
  slist socket str stralloc strarray strlist tai taia textbuf textcode uint16 uint32 uint64 unix var vartab wait xml ucs)

foreach(ARCHIVE ${MODSUBDIRS})
  file(GLOB MODHEADERS lib/${ARCHIVE}.h lib/${ARCHIVE}_*.h)
  file(GLOB MODSOURCES lib/${ARCHIVE}/*.c)
  if(NOT MODSOURCES)
  file(GLOB MODSOURCES lib/${ARCHIVE}_*.c)
  endif()
  add_library("${ARCHIVE}" ${MODSOURCES} ${MODHEADERS})
  set_target_properties("${ARCHIVE}" PROPERTIES
    PREFIX ""
    COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS}"
  )
endforeach(ARCHIVE)

foreach(NETLIB dns http io ndelay socket)
   target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()


target_link_libraries(unix stralloc buffer ndelay)
target_link_libraries(array safemult byte str)
target_link_libraries(binfmt mmap byte)
target_link_libraries(buffer byte fmt mmap open str ${ZLIB_LIBRARY})
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(dns byte case fmt io open socket str taia uint16 ${LIBIPHLPAPI})
target_link_libraries(dir stralloc)
target_link_libraries(errmsg str)
target_link_libraries(expand errmsg var unix)
target_link_libraries(elf byte str range mmap uint64 uint32)
target_link_libraries(fmt byte str uint64 uint32)
target_link_libraries(gpio mmap)
target_link_libraries(hmap str)
target_link_libraries(http dns errmsg byte io scan socket stralloc str ${OPENSSL_LIBARRIES})
target_link_libraries(io buffer iarray array byte fmt open taia ${LIBMSWSOCK})
target_link_libraries(json charbuf hmap scan)
target_link_libraries(map str)
target_link_libraries(mmap open scan ${LIBPSAPI})
target_link_libraries(open str uint32)
target_link_libraries(path unix dir strlist)
target_link_libraries(pe uint64 uint32 uint16)
target_link_libraries(playlist byte fmt scan str xml)
target_link_libraries(rdir dir)
target_link_libraries(scan byte str)
target_link_libraries(socket byte fmt ndelay scan uint16 ${LIBMSWSOCK})
target_link_libraries(str byte)
target_link_libraries(stralloc open byte fmt str)
target_link_libraries(strarray array)
target_link_libraries(strlist stralloc buffer)
target_link_libraries(taia tai)
target_link_libraries(textbuf byte playlist)
target_link_libraries(uint64 uint32)
target_link_libraries(xml buffer array strlist hmap alloc)
target_link_libraries(json slist hmap stralloc buffer alloc)
target_link_libraries(ndelay socket)
target_link_libraries(process wait errmsg)
target_link_libraries(ihex scan byte)
target_link_libraries(uint32 taia tai)

set(genmakefile_SOURCES mplab.c ini.c)

if(OPENSSL_LIB_DIR)
  set(CMAKE_EXE_LINKER_FLAGS "-L${OPENSSL_LIB_DIR}")
endif()



set(COMMON_LIBS unix strlist array safemult scan str byte mmap open fmt )
file(GLOB MODHEADERS lib/*.h)
function(add_program PROG)
  unset(INCLUDES)
  foreach(DEP ${ARGN})
  list(FIND MODSUBDIRS "${DEP}" I)
  if(I GREATER -1)
  list(APPEND INCLUDES "lib/${DEP}.h")
  endif(I GREATER -1)
  endforeach(DEP ${ARGN})
  #message("Program '${PROG}' dependencies: ${INCLUDES}")
  add_executable(${PROG} ${PROG}.c ${${PROG}_SOURCES} ${INCLUDES})
  target_link_libraries(${PROG} ${ARGN})
  install(TARGETS ${PROG} DESTINATION bin)
  set_target_properties(${PROG} PROPERTIES LINK_FLAGS "-L${OPENSSL_LIB_DIR} -Wl,-rpath=${OPENSSL_LIB_DIR}")
  if(EMSCRIPTEN)
    set_target_properties(${PROG} PROPERTIES SUFFIX ".html")
  endif(EMSCRIPTEN)
endfunction(add_program PROG)



if(NOT LIBLZMA_LIBRARY)
  set(LIBLZMA_LIBRARY "${LIBLZMA_LIBRARY}")
endif(NOT LIBLZMA_LIBRARY)

set(COMPRESSION_LIBRARIES
  ${LIBLZMA_LIBRARY}
  ${ZLIB_LIBRARY}
  ${BZIP2_LIBRARY}
)

if(OPENSSL_LIBRARIES)
  set(CRYPTO_LIBRARIES ${OPENSSL_LIBRARIES} pthread ${LIBCOMPAT} )
endif(OPENSSL_LIBRARIES)

add_program(binfmttest binfmt)
add_program(bsdiffcat buffer array safemult byte errmsg uint32 uint64 ${COMPRESSION_LIBRARIES})
add_program(buffertest stralloc buffer array case mmap ${COMPRESSION_LIBRARIES})
add_program(ccat buffer open str unix ${COMPRESSION_LIBRARIES})
add_program(compiler-wrapper stralloc buffer byte fmt str strlist dir)
add_program(count-depth buffer fmt)
add_program(decode-ls-lR stralloc buffer byte fmt io open str)
add_program(dnsip dns stralloc buffer errmsg socket ${LIBIPHLPAPI})
add_program(dnsname dns stralloc buffer errmsg ${LIBIPHLPAPI})
add_program(dnstest dns stralloc buffer errmsg ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(eagle-gen-cmds xml stralloc buffer strarray array byte cb cbmap errmsg fmt hmap mmap scan str unix)
add_program(eagle-init-brd xml stralloc buffer hmap strlist  scan str )
add_program(eagle-to-circuit xml stralloc buffer strlist array byte cb cbmap fmt hmap mmap scan str ${LIBMATH})
add_program(eagle-to-svg xml stralloc buffer strlist array byte cb cbmap fmt hmap mmap scan str)
add_program(elf64list elf mmap uint16 uint32 uint64)
add_program(elflist buffer errmsg unix elf mmap str)
add_program(elfwrsec buffer elf mmap)
add_program(genmakefile case errmsg strarray slist rdir dir path strlist hmap stralloc buffer mmap unix open scan fmt byte str array safemult alloc)
add_program(hexedit stralloc buffer array io errmsg mmap open path scan uint64 unix ${ZLIB_LIBRARY})
add_program(httptest http dns stralloc buffer errmsg byte io socket alloc taia ${LIBMSWSOCK} ${CRYPTO_LIBRARIES} pthread)
add_program(impgen buffer byte mmap open pe uint16 uint32)
add_program(jsontest json stralloc buffer byte charbuf fmt hmap mmap open)
add_program(list-r strarray strlist stralloc buffer array unix dir array fmt open str uint64 ${LIBADVAPI32})
add_program(macho32list mmap uint32)
add_program(mediathek-list errmsg stralloc buffer byte fmt http scan slist str unix strarray strlist ${LIBLZMA_LIBRARY} ${CRYPTO_LIBRARIES} pthread)
add_program(mediathek-parser ucs errmsg stralloc buffer array byte open str unix strlist)
add_program(ntldd stralloc buffer pe strlist byte path str unix uint64)
add_program(opensearch-dump xml stralloc buffer)
add_program(pathtool stralloc buffer errmsg path strlist unix)
add_program(pelist buffer errmsg mmap pe str unix)
add_program(omflist stralloc buffer errmsg omf mmap str unix)
add_program(pkgcfg stralloc buffer byte cbmap dir env errmsg path slist str unix strarray strlist)
add_program(plsconv stralloc buffer byte open playlist str unix)
add_program(rdir-test rdir stralloc buffer)
add_program(reg2cmd stralloc buffer byte fmt open scan str uint64)
add_program(regfilter stralloc buffer byte fmt open scan str uint64)
add_program(sln unix stralloc buffer byte errmsg path)
add_program(strarraytest xml dns stralloc buffer mmap strarray unix)
add_program(torrent-progress stralloc buffer io mmap open uint64)
add_program(xmlpp errmsg xml stralloc buffer hmap io unix)
add_program(xmltest xml stralloc buffer byte fmt hmap)
add_program(xmltest2 xml stralloc buffer byte fmt hmap)
add_program(xmltest3 xml stralloc buffer byte fmt hmap)
add_program(xmltest4 xml stralloc buffer array byte cb cbmap fmt hmap mmap scan str strlist)
add_program(ziptest buffer str unix ${COMPRESSION_LIBRARIES})
add_program(cc-wrap errmsg env buffer path dir strarray strlist stralloc fmt)
add_program(ar-wrap path process errmsg env buffer dir strarray strlist stralloc fmt alloc)
add_program(cofflist coff unix errmsg stralloc buffer mmap)
add_program(msys-shell unix errmsg env buffer path strarray strlist stralloc)
add_program(tcping map unix dns case io array socket ndelay errmsg taia tai buffer stralloc mmap open fmt scan str byte uint16 ${LIBMSWSOCK} ${LIBIPHLPAPI})
add_program(crc errmsg buffer mmap open ${ZLIB_LIBRARY})
add_program(cmake-run errmsg env buffer path dir strarray strlist stralloc fmt)

add_program(jsonpp json errmsg unix)
add_program(xml2json json xml errmsg unix)
add_program(xml2moon xml textcode errmsg)
add_program(strip-comments  errmsg env charbuf buffer path dir strarray strlist stralloc fmt)

add_program(httpproxy http io map)
#add_program(tinyproxy http io)
#if(NOT WIN32)
#add_program(tcpproxy http io)
#endif(NOT WIN32)
#add_program(redir http io ${LIBCOMPAT})

add_program(parse strlist errmsg)
add_program(testihex ihex stralloc buffer alloc mmap)

add_subdirectory(tests)
add_subdirectory(examples)
