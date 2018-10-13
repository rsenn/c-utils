set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)

project(dirlist)

set(BUILD_SHARED_LIBS FALSE)

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckTypeSize)
include(${CMAKE_CURRENT_SOURCE_DIR}/FindBZip2.cmake)
include(FindPkgConfig)
include(FindZLIB)
include(UsePkgConfig)
include(TestBigEndian)

include(${CMAKE_SOURCE_DIR}/CheckC11Generics.cmake)
include(${CMAKE_SOURCE_DIR}/CheckSizeTypes.cmake)
include(${CMAKE_SOURCE_DIR}/CheckAlloca.cmake)
include(${CMAKE_SOURCE_DIR}/CheckDynStack.cmake)
include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)

if(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS FALSE)
else(NOT ENABLE_SHARED)
  set(BUILD_SHARED_LIBS TRUE)
endif(NOT ENABLE_SHARED)

set(ignored_files
  "\\\\.git.*"
  "\\\\.o"
  "\\\\.log"
  "build/.*"
  "\\\\.swp\\\\$"
  "~\\\\$"
  "fuse_hidden"
  "core\\\\.[0-9]"
  "\\\\.vscode"
  "\\\\.codelite"
  "\\\\.settings"
  "CMakeFiles\\\\/"
  "\\\\.user"
)

set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL;/")


#include(CPack)

#include( "${CMAKE_CURRENT_SOURCE_DIR}/Checks.cmake" )

if(MSVC)
add_definitions(-Dinline=__inline)

  if(MSVC_VERSION STREQUAL 1500)
    message("Visual Studio 2008 disable warnings")
    add_definitions(/wd4013 /wd4024 /wd4047 /wd4100 /wd4201 /wd4244 /wd4255 /wd4668 /wd4706 /wd4820 /wd4996)
    add_definitions(-wd4018 -wd4022 -wd4028 -wd4057 -wd4101 -wd4131 -wd4189 -wd4200 -wd4206 -wd4214 -wd4242 -wd4245 -wd4273 -wd4333 -wd4389 -wd4700 -wd4702 -wd4716)
   endif()
endif(MSVC)
#if(WIN32 OR MSYS OR CYGWIN)
  add_definitions(-D_FILE_OFFSET_BITS=64)
#endif(WIN32 OR WIN64)
if(MSVC)
add_definitions(-wd4706)
else(MSVC)
add_definitions( -g  )
endif(MSVC)


set(CMAKE_C_FLAGS_DEBUG "-Wall -DDEBUG ${CMAKE_C_FLAGS_DEBUG}")

if(NOT MSVC)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 ${CMAKE_C_FLAGS_DEBUG}")
endif(NOT MSVC)
#set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O0 -Wall")

CHECK_C_COMPILER_FLAG("-ggdb" C_COMPILER_SUPPORTS_GGDB)

CHECK_C_COMPILER_FLAG(-Wno-unused-variable, W_NO_UNUSED_VAR)
if(W_NO_UNUSED_VAR)
  add_definitions(-Wno-unused-variable)
endif(W_NO_UNUSED_VAR)
CHECK_C_COMPILER_FLAG(-Wno-unused-parameter, W_NO_UNUSED_PARAM)
if(W_NO_UNUSED_PARAM)
  add_definitions(-Wno-unused-parameter)
endif(W_NO_UNUSED_PARAM)


if(C_COMPILER_SUPPORTS_GGDB)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -ggdb")
endif(C_COMPILER_SUPPORTS_GGDB)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -DDEBUG")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O0 -Wall")

CHECK_CXX_COMPILER_FLAG("-ggdb" CXX_COMPILER_SUPPORTS_GGDB)

if(CXX_COMPILER_SUPPORTS_GGDB)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -ggdb")
endif(CXX_COMPILER_SUPPORTS_GGDB)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

include_directories("${CMAKE_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}")
#include_directories("${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

#pkg_search_module(LIBLZMA liblzma)

if(MINGW OR CYGWIN OR MSYS OR WIN32)
  set(BIG_ENDIAN FALSE)
else(MINGW OR CYGWIN OR MSYS OR WIN32)
  TEST_BIG_ENDIAN(BIG_ENDIAN)
endif(MINGW OR CYGWIN OR MSYS OR WIN32)

if(BIG_ENDIAN)
  add_definitions(-D__BIG_ENDIAN__=1)
else(BIG_ENDIAN)
  add_definitions(-D__LITTLE_ENDIAN__=1)
endif(BIG_ENDIAN)



if(LIBLZMA_FOUND)
  link_libraries(${LIBLZMA_LIBRARIES})
  include_directories(${LIBLZMA_INCLUDE_DIRS})
  add_definitions(-DHAVE_LIBLZMA=1
   -DLZMA_API_STATIC=1
  )
else()
  add_definitions(-DHAVE_LIBLZMA=0)
endif()

if(ZLIB_FOUND)
  link_libraries(${ZLIB_LIBRARIES})
  include_directories(${ZLIB_INCLUDE_DIRS})
  add_definitions(-DHAVE_ZLIB=1 #-DZLIB_DLL=1
  )
else()
  add_definitions(-DHAVE_ZLIB=0)
endif()

#if(BZIP2_FOUND)
  link_libraries(${BZIP2_LIBRARIES})
  include_directories(${BZIP2_INCLUDE_DIR})
#endif(BZIP2_FOUND)

#pkg_search_module(LIBZ zlib)
pkg_search_module(LIBBZ2 libbz2 bzip2)

if(LIBBZ2_FOUND OR BZIP2_FOUND)
  add_definitions(-DHAVE_LIBBZ2=1)
else()
  add_definitions(-DHAVE_LIBBZ2=0)
endif()


#check_library_exists(LIBMSWSOCK ws2_32 HAVE_MSWSOCK)
check_library_exists(ws2_32 socket "" HAVE_MSWSOCK)
if(NOT HAVE_MSWSOCK)
  check_library_exists(ws2_32 _socket "" HAVE_MSWSOCK)
endif(NOT HAVE_MSWSOCK)
if(NOT HAVE_MSWSOCK)
  check_library_exists(ws2_32 __imp_socket "" HAVE_MSWSOCK)
endif(NOT HAVE_MSWSOCK)
if(NOT HAVE_MSWSOCK)
  check_library_exists(ws2_32 _imp_socket "" HAVE_MSWSOCK)
endif(NOT HAVE_MSWSOCK)

if(HAVE_MSWSOCK)
  set(LIBMSWSOCK ws2_32)
endif()
if(MINGW OR MINGW32 OR WIN32 OR WIN64)
  set(LIBIPHLPAPI iphlpapi)
  set(LIBPSAPI psapi)
endif()

if(NOT CXX11_FLAGS)
set(CXX11_FLAGS "-std=c++11")
endif()

if(CXX11_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
#  add_definitions(-std=c++11)
endif()

if(HAS_CXX11_AUTO AND HAS_CXX11_NULLPTR AND HAS_CXX11_LAMBDA AND HAS_CXX11_STATIC_ASSERT AND HAS_CXX11_RVALUE_REFERENCES AND HAS_CXX11_DECLTYPE AND HAS_CXX11_CSTDINT_H AND HAS_CXX11_LONG_LONG)
  message(STATUS "Enabling C++11 support")
  set(CXX11 TRUE)
endif()



check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(libgen.h HAVE_LIBGEN_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
#check_include_file(boost/foreach.hpp HAVE_BOOST_FOREACH_HPP)
#check_include_file(xtr1common HAVE_XTR1COMMON)
check_include_file(errno.h HAVE_ERRNO_H)
#check_include_file(initializer_list HAVE_INITIALIZER_LIST)

if(HAVE_ERRNO_H)
  add_definitions(-DHAVE_ERRNO_H=1)
endif()

#check_type_size(loff_t LOFF_T)
#check_type_size(lloff_t LLOFF_T)
check_type_size(off64_t OFF64_T)
check_type_size(socklen_t SOCKLEN_T)

if(NOT SOCKLEN_T)
#  add_definitions(-Dsocklen_t=int)
endif()


foreach(FUNC posix_memalign aligned_alloc _aligned_malloc alloca fnmatch inet_pton llseek lseek lseek64 lstat pipe2 round wordexp WSAStartup popen cygwin_conv_path sigemptyset sigaddset sigprocmask waitpid)
  string(TOUPPER "HAVE_${FUNC}" DEF)
  check_function_exists(${FUNC} ${DEF})
  if(${DEF})
    add_definitions(-D${DEF}=1)
  endif()
endforeach()

function(check_fn NAME)
  string(TOUPPER "HAVE_${NAME}" HAVE_${NAME}_VAR)
  check_function_exists(${NAME} HAVE_${NAME}_FUNC)
  if(HAVE_${NAME}_FUNC)
    add_definitions(-D${HAVE_${NAME}_VAR}=1)
  endif(HAVE_${NAME}_FUNC)
endfunction(check_fn NAME)


check_fn(aligned_alloc)
check_fn(_aligned_malloc)

#check_fn(stat)
check_fn(lstat)
check_fn(_llseek)
check_fn(_lseeki64)
check_fn(lseek)
check_fn(llseek)
check_fn(lseek64)
#check_fn(localtime)
check_fn(localtime_r)
check_fn(setmode)
check_fn(strncasecmp)
check_fn(strnicmp)
check_fn(strcasecmp)
check_fn(stricmp)
check_fn(popen)
check_fn(cygwin_conv_path)

check_fn(sigwaitinfo)
check_fn(sigtimedwait)
check_fn(inet_pton)

if(NOT HAVE_INET_PTON)
check_library_exists(ws2_32 inet_pton "" HAVE_INET_PTON)
endif()
if(NOT HAVE_INET_PTON)
check_library_exists(ws2_32 _inet_pton "" HAVE_INET_PTON)
endif()

if(HAVE_INET_PTON)
# message(IPv6 support: yes)
# add_definitions(-DLIBC_HAS_IP6=1)
endif()

if(NOT HAVE_strncasecmp_FUNC AND HAVE_strnicmp_FUNC)
add_definitions(-Dstrncasecmp=strnicmp)
endif(NOT HAVE_strncasecmp_FUNC AND HAVE_strnicmp_FUNC)

if(NOT HAVE_strcasecmp_FUNC AND HAVE_stricmp_FUNC)
add_definitions(-Dstrcasecmp=stricmp)
endif(NOT HAVE_strcasecmp_FUNC AND HAVE_stricmp_FUNC)

if(HAVE_setmode_FUNC)
 add_definitions(-DHAVE_SETMODE=1)
 endif()

 #foreach(SEEK_FN LSEEK64 _LLSEEK _LLSEEKI64 LLSEEK io_seek)
 foreach(SEEK_FN io_seek LLSEEK _LLSEEKI64 _LLSEEK LSEEK64)
  if(HAVE_${SEEK_FN}_FUNC)
    string(TOLOWER "${SEEK_FN}" SEEK_FUNCTION)

    add_definitions(-DHAVE_${SEEK_FN}=1)
  endif()
endforeach()

check_library_exists(ws2_32 WSAStartup "" HAVE_MSWSOCK_LIBRARY)
if(NOT HAVE_MSWSOCK_LIBRARY)
  check_library_exists(ws2_32 _WSAStartup "" HAVE_MSWSOCK_LIBRARY)
endif(NOT HAVE_MSWSOCK_LIBRARY)

if(HAVE_MSWSOCK_LIBRARY OR MINGW OR MINGW32 OR MINGW64 OR MSVC)
  set(LIBMSWSOCK ws2_32)
  #link_libraries(${LIBMSWSOCK})
endif()


IF(MSVC OR WIN32 OR WIN64 OR MINGW OR MINGW32 OR MINGW64)
  SET(SOCKET_LIB ws2_32)
  set(LIBPSAPI psapi)
endif()

IF(MINGW)
  list(APPEND CMAKE_EXE_LINKER_FLAGS "-static-libgcc")
endif(MINGW)

#list(APPEND ${LIBMSWSOCK} ${LIBLZMA_LIBRARY} ${LIBZ_LIBRARY} ${LIBBZ2_LIBRARY})

if(MINGW OR MINGW32 OR MINGW64 OR MSVC OR WIN32 OR WIN64)
  if(NOT LIBMSWSOCK)
    set(LIBMSWSOCK ws2_32)
  endif()
  #link_libraries(${LIBMSWSOCK})
endif()



if(SEEK_FUNCTION)
  add_definitions(-Dio_seek=${SEEK_FUNCTION})
endif(SEEK_FUNCTION)
#if(HAVE_lseek64_FUNC)
#  add_definitions(-Dio_seek=lseek64)
#elseif(HAVE__llseek_FUNC)
#  add_definitions(-Dio_seek=_llseek)
#elseif(HAVE__lseeki64_FUNC)
#  add_definitions(-Dio_seek=_lseeki64)
#elseif(HAVE_llseek_FUNC)
#  add_definitions(-Dio_seek=llseek)
#
#elseif(HAVE_LSEEK)
#  add_definitions(-Dio_seek=lseek)
#endif()

check_function_exists(free HAVE_free_FUNC)
check_function_exists(malloc HAVE_malloc_FUNC)
check_function_exists(opendir HAVE_opendir_FUNC)
check_function_exists(readdir HAVE_readdir_FUNC)

check_function_exists(getdents64 HAVE_getdents64_FUNC)
check_function_exists(getdents HAVE_getdents_FUNC)

check_function_exists(wordexp HAVE_WORDEXP)
if(HAVE_WORDEXP)
  add_definitions(-DHAVE_WORDEXP=1)
endif()

check_function_exists(posix_memalign HAVE_POSIX_MEMALIGN)
if(HAVE_POSIX_MEMALIGN)
  add_definitions(-DHAVE_POSIX_MEMALIGN=1)
endif()

if(HAVE_lstat_FUNC)
if(NOT MINGW AND NOT MSYS)
  if(HAVE_opendir_FUNC AND HAVE_readdir_FUNC)
    set(USE_READDIR TRUE)
  else(HAVE_opendir_FUNC AND HAVE_readdir_FUNC)
    set(USE_READDIR FALSE)
  endif(HAVE_opendir_FUNC AND HAVE_readdir_FUNC)
  endif()
endif(HAVE_lstat_FUNC)

check_function_exists(getopt_long HAVE_getopt_long_FUNC)
check_function_exists(getopt HAVE_getopt_FUNC)

if(NOT HAVE_getopt_long_FUNC OR NOT HAVE_getopt_FUNC)
  set(GETOPT_SOURCES ${CMAKE_SOURCE_DIR}/lib/getopt.c)
endif()

#check_library_exists(m, atan2 "" HAVE_MATH_LIBRARY)
check_library_exists(m roundf "" HAVE_MATH_LIBRARY)

if(HAVE_MATH_LIBRARY)
  set(MATH_LIBRARY m)
else()
  check_function_exists(atan2 HAVE_atan2_FUNC)
  check_function_exists(roundf HAVE_roundf_FUNC)

endif(HAVE_MATH_LIBRARY)




#include_directories( "${CMAKE_SOURCE_DIR}/lib" )
include_directories( "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")
include_directories( ${Iconv_INCLUDE_DIR} )

if(STATIC_LINK)
  add_definitions(-DNOLIBTOOL=1 -DLIBXML_STATIC=1)
else(STATIC_LINK)
  add_definitions(-DPIC=1)
endif(STATIC_LINK)


if(USE_READDIR)
  add_definitions(-DUSE_READDIR=1)
endif(USE_READDIR)

configure_file("${CMAKE_SOURCE_DIR}/config.h.cmake"
               "${CMAKE_BINARY_DIR}/config.h")

#configure_file("${CMAKE_SOURCE_DIR}/lib/buffer.h.cmake" "${CMAKE_BINARY_DIR}/lib/buffer.h")
#configure_file("${CMAKE_SOURCE_DIR}/lib/charbuf.h.cmake" "${CMAKE_BINARY_DIR}/lib/charbuf.h")


file(GLOB COMMON_C_SOURCES RELATIVE "${CMAKE_SOURCE_DIR}" "*_*.c" "umult*.c" "*.h")
set(COMMON_CXX_SOURCES lib/directory_iterator.cpp lib/file.cpp lib/intelhex.cpp)

set(PROGRAMS
  access binfmttest bsdiffcat buffertest ccat count-depth decode-ls-lR dnsip dnsname dnstest eagle-gen-cmds eagle-init-brd eagle-to-circuit elf64list elflist elfwrsec genmakefile hexedit httptest impgen jsontest list-r macho32list mediathek-list mediathek-parser ntldd opensearch-dump pathtool pelist pkgcfg plsconv rdir-test reg2cmd regfilter sln strarraytest torrent-progress xmlpp xmltest xmltest2 xmltest3 xmltest4 ziptest
)

set(CPACK_PACKAGE_EXECUTABLES ${PROGRAMS})

set(CXXPROGRAMS #kbd-adjacency
#ls-R
 #   piccfghex
 )

if(MSVC)
#  add_definitions(-Dinline=__force_inline)
  add_definitions(/DINLINE=__inline)
  add_definitions(/wo4005)
  add_definitions(/W0)
else()
  add_definitions(-DINLINE=inline)
endif()

if(CXX11)
#  add_definitions(-DCXX11=1)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS} -DCXX11=1")
endif()

string(REGEX REPLACE "/W[0-9]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


add_definitions(-DHAVE_CONFIG_H)

set(HAVE_VARS HAVE_ALLOCA_H HAVE_BOOST_FOREACH_HPP HAVE_INTTYPES_H HAVE_LIBGEN_H HAVE_STDINT_H HAVE_SYS_MMAN_H HAVE_SYS_STAT_H HAVE_SYS_TYPES_H)

foreach(V ${HAVE_VARS})
  if(${V})
    add_definitions(-D${V}=1)
  endif()
endforeach()


#file(GLOB LIBOWFAT_SOURCES lib/*.c lib/*.h)
#
#add_library(libowfat ${LIBOWFAT_SOURCES})
#
#if("${CMAKE_STATIC_LIBRARY_PREFIX}" STREQUAL "lib")
#    set_target_properties(libowfat PROPERTIES
#        ARCHIVE_OUTPUT_NAME owfat
#        LIBRARY_OUTPUT_NAME owfat
#    )
#endif()


#
set(LIBSUBDIRS array binfmt buffer byte case cb cbmap charbuf dir dns elf env errmsg expand fmt gpio hmap http iarray io json list map mmap ndelay open path pe playlist rdir scan sig slist socket str stralloc strarray strlist tai taia textbuf uint16 uint32 uint64 var vartab xml)

foreach(ARCHIVE ${LIBSUBDIRS})
  file(GLOB MODHEADERS lib/${ARCHIVE}.h lib/${ARCHIVE}_*.h)
  file(GLOB MODSOURCES lib/${ARCHIVE}/*.c)
  if(NOT MODSOURCES)
    file(GLOB MODSOURCES lib/${ARCHIVE}_*.c)
  endif()

  if(ARCHIVE STREQUAL io)
    list(APPEND MODSOURCES lib/iopause.c)
  endif()
  if(ARCHIVE STREQUAL str)
    list(APPEND MODSOURCES lib/time_table_spd.c lib/isleap.c)
  endif()
  if(ARCHIVE STREQUAL array)
    list(APPEND MODSOURCES lib/umult64.c)
  endif()
  if(ARCHIVE STREQUAL dir)
    list(APPEND MODSOURCES lib/fnmatch.c lib/utf8.c)
  endif()
  if(ARCHIVE STREQUAL io)
    list(APPEND MODSOURCES lib/iopause.c)
  endif()
  if(ARCHIVE STREQUAL path)
    list(APPEND MODSOURCES lib/readlink.c lib/symlink.c lib/wordexp.c lib/pipe2.c lib/getdelim.c)
  endif()
  if(ARCHIVE STREQUAL open)
    list(APPEND MODSOURCES lib/popen.c)
  endif()
  if(ARCHIVE STREQUAL array)
    list(APPEND MODSOURCES lib/umult64.c)
  endif()
  if(ARCHIVE STREQUAL cbmap)
    list(APPEND MODSOURCES lib/memalign.c)
  endif()
  if(ARCHIVE STREQUAL env)
    list(APPEND MODSOURCES lib/setenv.c lib/getenv.c)
  endif()
  if(ARCHIVE STREQUAL expand)
    list(APPEND MODSOURCES lib/glob.c)
  endif()
#  if(ARCHIVE STREQUAL socket)
#    list(APPEND MODSOURCES lib/socket/winsock2errno.c lib/socket/winsock_init.c)
#  endif()

  add_library("${ARCHIVE}" ${MODSOURCES} ${MODHEADERS})
  set_target_properties("${ARCHIVE}" PROPERTIES PREFIX "")
endforeach(ARCHIVE)

foreach(NETLIB dns http io ndelay socket)
  target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()

target_link_libraries(array byte str)
target_link_libraries(buffer array byte fmt mmap open str stralloc)
target_link_libraries(byte stralloc)
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(dns byte case fmt io open socket str stralloc taia uint16)
target_link_libraries(errmsg str)
target_link_libraries(fmt byte str)
target_link_libraries(gpio buffer)
target_link_libraries(hmap buffer str)
target_link_libraries(http buffer byte io scan socket str stralloc)
target_link_libraries(io array byte fmt iarray open taia)
target_link_libraries(json buffer byte charbuf hmap scan stralloc)
target_link_libraries(map str)
target_link_libraries(mmap open ${LIBPSAPI})
target_link_libraries(open stralloc)
target_link_libraries(pe uint64 uint32 uint16)
target_link_libraries(playlist buffer byte fmt scan str stralloc xml)
target_link_libraries(scan byte case str)
target_link_libraries(socket byte fmt ndelay scan uint16)
target_link_libraries(str byte stralloc)
target_link_libraries(stralloc buffer byte fmt str)
target_link_libraries(strarray array byte str)
target_link_libraries(strlist buffer byte fmt str stralloc)
target_link_libraries(taia tai)
target_link_libraries(textbuf buffer byte playlist)
target_link_libraries(uint64 uint32)
target_link_libraries(xml array buffer byte fmt hmap scan str stralloc strlist)


if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  if(NOT UNIX)
#    set(X64 "-x64")
  endif()
endif()

set(COMMON_LIBS strlist buffer array stralloc scan str byte mmap open fmt )

file(GLOB LIBHEADERS lib/*.h)

foreach(PROGRAM ${PROGRAMS})
  if("${PROGRAM}" STREQUAL ntldd)
    set(EXTRA_SOURCES libntldd.c)
  else("${PROGRAM}" STREQUAL ntldd)
    set(EXTRA_SOURCES )
  endif("${PROGRAM}" STREQUAL ntldd)
  
  if("${PROGRAM}" STREQUAL "ccat" OR "${PROGRAM}" STREQUAL "eagle-gen-cmds" OR "${PROGRAM}" STREQUAL "genmakefile" OR "${PROGRAM}" STREQUAL "list-r" OR "${PROGRAM}" STREQUAL "mediathek-list" OR "${PROGRAM}" STREQUAL "mediathek-parser" OR "${PROGRAM}" STREQUAL "ntldd" OR "${PROGRAM}" STREQUAL "pathtool" OR "${PROGRAM}" STREQUAL "pelist" OR "${PROGRAM}" STREQUAL "pkgcfg" OR "${PROGRAM}" STREQUAL "plsconv" OR "${PROGRAM}" STREQUAL "strarraytest" OR "${PROGRAM}" STREQUAL "ziptest")
    list(APPEND EXTRA_SOURCES ${GETOPT_SOURCES})
  endif()

  add_executable(${PROGRAM} ${PROGRAM}.c ${EXTRA_SOURCES} ${LIBHEADERS}) #  get_target_property(${PROGRAM}_INCLUDE_DIRECTORIES ${PROGRAM} INCLUDE_DIRECTORIES)
 # list(APPEND ${PROGRAM}_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR})

  set_target_properties(${PROGRAM} PROPERTIES
    OUTPUT_NAME "${PROGRAM}${X64}"
    INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR};${CMAKE_SOURCE_DIR}"
    )
#  target_link_libraries(${PROGRAM} ${COMMON_LIBS})
  install(TARGETS ${PROGRAM} DESTINATION bin)
endforeach(PROGRAM)

target_link_libraries(list-r dir ${COMMON_LIBS})
target_link_libraries(genmakefile rdir dir ${COMMON_LIBS})
target_link_libraries(rdir-test rdir dir ${COMMON_LIBS})

target_link_libraries(access buffer str)
target_link_libraries(buffertest buffer str ${LIBPSAPI})
target_link_libraries(ccat buffer str)
#target_link_libraries(compiler-wrapper buffer byte dir fmt str stralloc strlist)
target_link_libraries(count-depth buffer)
target_link_libraries(decode-ls-lR buffer byte playlist str stralloc)
target_link_libraries(dnsip buffer dns errmsg socket str stralloc ${LIBIPHLPAPI})
target_link_libraries(dnsname buffer dns errmsg socket ${LIBIPHLPAPI})
target_link_libraries(dnstest buffer dns errmsg socket ${LIBIPHLPAPI})
target_link_libraries(eagle-gen-cmds array buffer byte cbmap scan str stralloc strarray strlist xml ${MATH_LIBRARY})
target_link_libraries(eagle-init-brd buffer hmap scan str stralloc strlist xml ${MATH_LIBRARY})
target_link_libraries(eagle-to-circuit array buffer byte cbmap scan str stralloc strlist xml)
target_link_libraries(elfwrsec elf buffer mmap str uint64 uint32)
target_link_libraries(elflist elf buffer mmap str uint64 uint32 uint16)
target_link_libraries(hexedit  errmsg buffer path stralloc str mmap open scan)
target_link_libraries(httptest buffer byte http dns errmsg io taia ${LIBIPHLPAPI})
target_link_libraries(impgen buffer byte mmap)
target_link_libraries(jsontest buffer charbuf fmt json open str stralloc)
target_link_libraries(list-r array buffer dir fmt open str stralloc)
target_link_libraries(mediathek-list buffer byte fmt open scan str stralloc strlist)
target_link_libraries(mediathek-parser  io iarray array buffer byte open playlist str stralloc)
target_link_libraries(ntldd path pe strlist buffer fmt str uint64 uint32 uint16)
target_link_libraries(pathtool errmsg path buffer stralloc strlist)
target_link_libraries(plsconv buffer byte open playlist str stralloc)
target_link_libraries(rdir-test buffer rdir)
target_link_libraries(reg2cmd buffer scan str stralloc)
target_link_libraries(regfilter buffer scan str stralloc)
target_link_libraries(torrent-progress buffer mmap open)
target_link_libraries(xmlpp buffer hmap  stralloc xml)
target_link_libraries(xmltest buffer fmt  str stralloc xml)
target_link_libraries(xmltest2 buffer fmt  str stralloc xml)
target_link_libraries(xmltest3 buffer fmt  str stralloc xml)
target_link_libraries(xmltest4 array buffer byte cbmap scan str stralloc strlist xml)
target_link_libraries(ziptest buffer str ${LIBBZ2_LIBRARY} )
target_link_libraries(pkgcfg array buffer byte cbmap dir errmsg env path ndelay socket slist str stralloc strarray strlist)
target_link_libraries(sln buffer byte errmsg path  str stralloc)

target_link_libraries(opensearch-dump xml errmsg uint64 uint32 uint16 buffer mmap open)
target_link_libraries(bsdiffcat errmsg uint64 uint32 uint16 buffer mmap open)
target_link_libraries(binfmttest binfmt errmsg uint64 uint32 uint16 buffer mmap open)
target_link_libraries(elf64list errmsg uint64 uint32 uint16 buffer mmap open)
target_link_libraries(macho32list errmsg uint64 uint32 uint16 buffer mmap open)
target_link_libraries(pelist pe errmsg uint64 uint32 uint16 buffer mmap open)
target_link_libraries(strarraytest strarray stralloc array buffer str)



#target_link_libraries(eagle-init-brd)

#if(Boost_INCLUDE_DIRS)
#  include_directories("${Boost_INCLUDE_DIRS}")
#endif()

foreach(CXXPROGRAM ${CXXPROGRAMS})
#  add_executable( ${CXXPROGRAM} "${CMAKE_SOURCE_DIR}/${CXXPROGRAM}.cpp" ${COMMON_CXX_SOURCES} ${COMMON_C_SOURCES} )
  install(TARGETS ${CXXPROGRAM} DESTINATION bin)
endforeach(CXXPROGRAM)
