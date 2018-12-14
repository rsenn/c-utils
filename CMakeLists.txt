set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_WARN_VS8 OFF)

project(dirlist C)

set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)
set(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL)


set(BUILD_SHARED_LIBS FALSE)

if(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS FALSE)
else(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS TRUE)
endif(NOT ENABLE_SHARED)

include(FindPkgConfig)

#include(${CMAKE_SOURCE_DIR}/FindZlib.cmake)
#include(${CMAKE_SOURCE_DIR}/FindBZip2.cmake)
#include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)
#include(${CMAKE_SOURCE_DIR}/FindOpenSSL.cmake)

include(${CMAKE_SOURCE_DIR}/CheckInline.cmake)

set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_MINSIZEREL)
foreach(CompilerFlag ${CompilerFlags})
  unset(FLAG)
  string(REPLACE "/MD" "/MT" FLAG "${${CompilerFlag}}")
  #string(REPLACE "/MTd" "/MT" FLAG "${FLAG}")
  if("${CompilerFlag}" STREQUAL "CMAKE_C_FLAGS_DEBUG")
    set(FLAG "${FLAG} -DDEBUG=1")
  endif()
  string(REPLACE " " ";" FLAG "${FLAG}")
  list(REMOVE_DUPLICATES FLAG)
  string(REPLACE ";" " " FLAG "${FLAG}")
  unset(${CompilerFlag} CACHE)
    set(${CompilerFlag} "${FLAG}" CACHE STRING "Compiler flags ${CompilerFlag}")
  message("${CompilerFlag}: ${${CompilerFlag}}")
endforeach()


################################################################################
## Check LibLZMA
################################################################################
pkg_search_module(LIBLZMA liblzma lzma)
if(NOT LIBLZMA_FOUND)
include(FindLibLZMA)
endif(NOT LIBLZMA_FOUND)
if(LIBLZMA_FOUND)
  #link_libraries(${LIBLZMA_LIBRARIES})
  include_directories(${LIBLZMA_INCLUDE_DIRS})
  add_definitions(-DHAVE_LIBLZMA=1 -DLZMA_API_STATIC=1)
else(LIBLZMA_FOUND)
  add_definitions(-DHAVE_LIBLZMA=0)
endif(LIBLZMA_FOUND)

################################################################################
## Check Zlib
################################################################################
pkg_search_module(ZLIB zlib libz z)
if(NOT ZLIB_FOUND)
include(FindZlib)
endif(NOT ZLIB_FOUND)
if(ZLIB_FOUND)
  #link_libraries(${ZLIB_LIBRARIES})
  include_directories(${ZLIB_INCLUDE_DIRS})
  add_definitions(-DHAVE_LIBZ=1)
else(ZLIB_FOUND)
  add_definitions(-DHAVE_LIBZ=0)
endif(ZLIB_FOUND)

################################################################################
## Check BZip2
################################################################################
pkg_search_module(BZIP2 libbz2 bzip2)
if(NOT BZIP2_FOUND)
  include(FindBZip2)
endif(NOT BZIP2_FOUND)
if(LIBBZ2_FOUND OR BZIP2_FOUND)
  add_definitions(-DHAVE_LIBBZ2=1)
  include_directories(${BZIP2_INCLUDE_DIRS})
else(LIBBZ2_FOUND OR BZIP2_FOUND)
  add_definitions(-DHAVE_LIBBZ2=0)
endif(LIBBZ2_FOUND OR BZIP2_FOUND)

################################################################################
## Check OpenSSL
################################################################################
pkg_search_module(OPENSSL openssl libssl)
if(NOT OPENSSL_FOUND)
include(FindOpenSSL)
endif(NOT OPENSSL_FOUND)
if(OPENSSL_FOUND)
  add_definitions(-DHAVE_OPENSSL=1)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif(OPENSSL_FOUND)

#link_libraries(
#  ${ZLIB_LIBRARIES}
#  ${BZIP2_LIBRARIES}
#  ${LIBLZMA_LIBRARIES}
#  ${OPENSSL_LIBRARIES}
#  )
add_definitions(-Dinline=__inline)

option(USE_SELECT "Use select() syscall" OFF)

if(MINGW)
  set(LIBMSWSOCK ws2_32)
elseif(WIN32 OR WIN64 OR MSVC)
  set(LIBMSWSOCK wsock32)
endif()

if(USE_SELECT)
   add_definitions(-DUSE_SELECT=1)
endif(USE_SELECT)

set(LIBSUBDIRS array binfmt buffer byte case cb cbmap charbuf coff dir dns elf env errmsg expand fmt gpio hashmap hmap http iarray io json list map mmap ndelay omf open path pe playlist process range rdir safemult scan sig slist socket str stralloc strarray strlist tai taia textbuf textcode uint16 uint32 uint64 unix var vartab wait xml)

foreach(ARCHIVE ${LIBSUBDIRS})
  file(GLOB MODHEADERS lib/${ARCHIVE}.h lib/${ARCHIVE}_*.h)
  file(GLOB MODSOURCES lib/${ARCHIVE}/*.c)
  if(NOT MODSOURCES)
  file(GLOB MODSOURCES lib/${ARCHIVE}_*.c)
  endif()
  add_library("${ARCHIVE}" ${MODSOURCES} ${MODHEADERS})
  set_target_properties("${ARCHIVE}" PROPERTIES PREFIX "")
endforeach(ARCHIVE)

foreach(NETLIB dns http io ndelay socket)
   target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()


target_link_libraries(unix stralloc buffer ndelay)
target_link_libraries(array safemult byte str)
target_link_libraries(binfmt mmap byte)
target_link_libraries(buffer byte fmt mmap open str)
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(dns byte case fmt io open socket str taia uint16 ${LIBIPHLPAPI})
#target_link_libraries(dir )
target_link_libraries(errmsg str)
target_link_libraries(expand errmsg var unix)
target_link_libraries(elf byte str range mmap uint64 uint32)
target_link_libraries(fmt byte str uint64 uint32)
target_link_libraries(gpio mmap)
target_link_libraries(hmap str)
target_link_libraries(http dns errmsg byte io scan socket str ${OPENSSL_LIBRARIES})
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
target_link_libraries(strlist stralloc)
target_link_libraries(taia tai)
target_link_libraries(textbuf byte playlist)
target_link_libraries(uint64 uint32)
target_link_libraries(xml array strlist hmap)
target_link_libraries(ndelay socket)


set(COMMON_LIBS unix strlist array safemult scan str byte mmap open fmt )
file(GLOB LIBHEADERS lib/*.h)
function(add_program PROG)
  unset(INCLUDES)
  foreach(DEP ${ARGN})
  list(FIND LIBSUBDIRS "${DEP}" I)
  if(I GREATER -1)
  list(APPEND INCLUDES "lib/${DEP}.h")
  endif(I GREATER -1)
  endforeach(DEP ${ARGN})
  #message("Program '${PROG}' dependencies: ${INCLUDES}")
  add_executable(${PROG} ${PROG}.c ${INCLUDES})
  target_link_libraries(${PROG} ${ARGN})
  install(TARGETS ${PROG} DESTINATION bin)
endfunction(add_program PROG)

if(NOT LIBLZMA_LIBRARIES)
  set(LIBLZMA_LIBRARIES "${LIBLZMA_LIBRARY}")
endif(NOT LIBLZMA_LIBRARIES)


set(COMPRESSION_LIBRARIES
  ${LIBLZMA_LIBRARIES}
  ${ZLIB_LIBRARIES}
 ${BZIP2_LIBRARIES})


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
add_program(eagle-to-circuit xml stralloc buffer strlist  array byte cb cbmap fmt hmap mmap scan str)
add_program(elf64list elf mmap uint16 uint32 uint64)
add_program(elflist buffer errmsg unix elf mmap str)
add_program(elfwrsec buffer elf mmap)
add_program(genmakefile strarray stralloc buffer rdir errmsg hmap mmap path scan slist case str unix  strlist ${LIBSHLWAPI})
add_program(hexedit stralloc buffer array io errmsg mmap open path scan uint64 unix)
add_program(httptest http dns stralloc buffer errmsg byte io socket taia ${LIBMSWSOCK})
add_program(impgen buffer byte mmap open pe uint16 uint32)
add_program(jsontest json stralloc buffer byte charbuf fmt hmap mmap open)
add_program(list-r stralloc buffer unix dir array fmt open str uint64 ${LIBADVAPI32})
add_program(macho32list mmap uint32)
add_program(mediathek-list errmsg stralloc buffer byte fmt http scan slist str unix strarray strlist)
add_program(mediathek-parser errmsg stralloc buffer array byte open str unix strlist)
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
add_program(sln stralloc buffer byte errmsg path)
add_program(strarraytest xml dns stralloc buffer mmap strarray unix)
add_program(torrent-progress stralloc buffer io mmap open uint64)
add_program(xmlpp xml stralloc buffer hmap io)
add_program(xmltest xml stralloc buffer byte fmt hmap)
add_program(xmltest2 xml stralloc buffer byte fmt hmap)
add_program(xmltest3 xml stralloc buffer byte fmt hmap)
add_program(xmltest4 xml stralloc buffer array byte cb cbmap fmt hmap mmap scan str strlist)
add_program(ziptest buffer str unix ${COMPRESSION_LIBRARIES})
add_program(cc-wrap errmsg env buffer path dir strarray strlist stralloc fmt)
add_program(ar-wrap errmsg env buffer path dir strarray strlist stralloc fmt)
add_program(cofflist coff unix errmsg stralloc buffer mmap)
add_program(msys-shell unix errmsg env buffer path strarray strlist stralloc)
add_program(tcping map unix dns case io array socket ndelay errmsg taia tai buffer stralloc mmap open fmt scan str byte uint16 ${LIBMSWSOCK} ${LIBIPHLPAPI})

add_subdirectory(tests)
#add_subdirectory(examples)
