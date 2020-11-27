set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_WARN_VS8 OFF)

set(CMAKE_C_STANDARD 11)

project(c-utils C ASM)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation prefix" FORCE)
endif()
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)


SET(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type")

SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo) 

option(USE_LZMA "lzma compression" ON)
option(USE_BZIP "bzip2 compression" ON)
option(USE_ZLIB "zlib compression" ON)
option(USE_BROTLI "brotli compression" ON)
option(USE_SSL "ssl encryption" ON)

function(DUMP VAR)
  string(REGEX REPLACE "[;\n]" " " A "${ARGV}")
  message("\nVariable dump of: ${A}\n")

  foreach(VAR ${ARGV})
    message("  ${VAR} = ${${VAR}}")
  endforeach(VAR ${ARGV})
  message("")
endfunction(DUMP VAR)

if(CMAKE_C_COMPILER MATCHES "-musl")
  set(MUSL TRUE)
endif(CMAKE_C_COMPILER MATCHES "-musl")

if(CMAKE_C_COMPILER MATCHES diet)
  set(DIET TRUE)
endif()
include(CheckSymbolExists)

if(NOT DIETLIBC)
  check_symbol_exists(__dietlibc__ "" DIETLIBC)
  if(DIETLIBC)
    set(DIET TRUE)
  endif(DIETLIBC)
endif(NOT DIETLIBC)

if(NOT MUSL)
  check_symbol_exists(__musl__ "" MUSL)
endif(NOT MUSL)

if(DIET)
  set(TOOLCHAIN "diet")
else(DIET)
  if(MUSL)
    set(TOOLCHAIN "musl")
  endif(MUSL)
endif(DIET)

if(TOOLCHAIN STREQUAL "diet" OR TOOLCHAIN STREQUAL "musl")
  set(STATIC_LINK ON)
  set(SHARED_LIBS OFF)
else(TOOLCHAIN STREQUAL "diet" OR TOOLCHAIN STREQUAL "musl")
  set(STATIC_LINK OFF)
  set(SHARED_LIBS ON)
endif(TOOLCHAIN STREQUAL "diet" OR TOOLCHAIN STREQUAL "musl")

message(STATUS "Toolchain: ${TOOLCHAIN}")
if(WIN32 OR WINDOWS)
  option(USE_SELECT "Use select() syscall" ON)
else(WIN32 OR WINDOWS)
  option(USE_SELECT "Use select() syscall" OFF)
endif(WIN32 OR WINDOWS)

#[[option(USE_POLL "Use poll() syscall" OFF)
option(USE_EPOLL "Use epoll() syscall" OFF)]]
option(BUILD_SHARED_LIBS "Build shared libs" "${SHARED_LIBS}")
option(LINK_STATIC "Link libraries static" "${STATIC_LINK}")
option(ENABLE_WERROR "Enable -Werror (warnings are errors)" OFF)

macro(debug_option NAME DESC)
  option(DEBUG_${NAME} "${DESC}" OFF)
  if(DEBUG_${NAME})
    add_definitions(-DDEBUG_${NAME}=1)
  endif(DEBUG_${NAME})
endmacro(debug_option NAME DESC)

debug_option(IO "Debug I/O code")
debug_option(HTTP "Debug HTTP code")

if(DEBUG_IO)
  add_definitions(-DDEBUG_IO=1)
endif(DEBUG_IO)

if(ENABLE_WERROR)
  add_definitions(-Wall -Werror -Wno-unused)
endif(ENABLE_WERROR)

if(LINK_STATIC)
  set(DSO_NONE TRUE)
  set(CMAKE_SKIP_BUILD_RPATH TRUE)
  set(CMAKE_SKIP_INSTALL_RPATH TRUE)
  set(CMAKE_SKIP_RPATH TRUE)
  set(CMAKE_ENABLE_EXPORTS FALSE)
endif(LINK_STATIC)

set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.txt)
set(CPACK_INSTALL_CMAKE_PROJECTS ${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL)

# if(EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt") add_subdirectory(3rdparty/openssl) endif()

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
include(CheckCCompilerFlag)
include(CheckTypeSize)

macro(show_result RESULT_VAR)
  if(${${RESULT_VAR}})
    set(RESULT_VALUE yes)
  else(${${RESULT_VAR}})
    set(RESULT_VALUE no)
  endif(${${RESULT_VAR}})
endmacro(show_result RESULT_VAR)

macro(check_compile RESULT_VAR SOURCE)
  set(RESULT "${${RESULT_VAR}}")
  # message("${RESULT_VAR} = ${RESULT}" )
  if(RESULT STREQUAL "")
    string(RANDOM LENGTH 6 ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" C_NAME)
    string(REPLACE SUPPORT_ "" NAME "${RESULT_VAR}")
    string(REPLACE _ - NAME "${NAME}")
    string(TOLOWER "${NAME}" C_NAME)
    set(C_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/try-${C_NAME}.c")
    string(REPLACE "\\" "\\\\" SOURCE "${SOURCE}")
    file(WRITE "${C_SOURCE}" "${SOURCE}")
    message(CHECK_START "Trying to compile try-${C_NAME}.c ... ")
    try_compile(COMPILE_RESULT "${CMAKE_CURRENT_BINARY_DIR}" "${C_SOURCE}" OUTPUT_VARIABLE "OUTPUT" LINK_LIBRARIES "${ARGN}")
    # file(REMOVE "${C_SOURCE}")

    if(COMPILE_RESULT)
      message(CHECK_PASS "ok")
      # add_definitions(-D${RESULT_VAR})
    else(COMPILE_RESULT)
      set(COMPILE_LOG "${CMAKE_CURRENT_BINARY_DIR}/compile-${C_NAME}.log")
      message(CHECK_FAIL "fail: ${COMPILE_LOG}")
      file(WRITE "${COMPILE_LOG}" "${OUTPUT}")
      string(REPLACE "\n" ";" OUTPUT "${OUTPUT}")
      list(FILTER OUTPUT INCLUDE REGEX "error")
    endif(COMPILE_RESULT)

    set("${RESULT_VAR}" "${COMPILE_RESULT}" CACHE BOOL "Support ${NAME}")
  endif(RESULT STREQUAL "")
  show_result(${RESULT_VAR})
endmacro(check_compile RESULT_VAR SOURCE)
macro(check_run RESULT_VAR SOURCE)
  string(RANDOM LENGTH 6 ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" C_SOURCE)
  set(C_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/try-${C_SOURCE}.c")
  string(REPLACE "\\" "\\\\" SOURCE "${SOURCE}")
  file(WRITE "${C_SOURCE}" "${SOURCE}")
  try_run(RUN_RESULT COMPILE_RESULT "${CMAKE_CURRENT_BINARY_DIR}" "${C_SOURCE}" RUN_OUTPUT_VARIABLE RUN_OUTPUT COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT)

  if("${RUN_RESULT}" STREQUAL 0)
    set(RUN_OK TRUE)
  else("${RUN_RESULT}" STREQUAL 0)
    set(RUN_OK FALSE)
  endif("${RUN_RESULT}" STREQUAL 0)

  # message("Run result: ${RUN_OK} output: ${RUN_OUTPUT}") message("Compile result: ${COMPILE_RESULT}")

  file(REMOVE "${C_SOURCE}")

  if(NOT COMPILE_RESULT)
    set("${RESULT_VAR}" "${COMPILE_RESULT}")
    set(RUN_RESULT FALSE)
  else(NOT COMPILE_RESULT)
    set("${RESULT_VAR}" "${RUN_RESULT}")
  endif(NOT COMPILE_RESULT)

  if(RUN_OK)
    add_definitions(-D${RESULT_VAR})
  else(RUN_OK)
    string(REPLACE "\n" ";" OUTPUT "${OUTPUT}")
    list(FILTER OUTPUT INCLUDE REGEX "error")
  endif(RUN_OK)

  if(ARGN)
    set("${ARGN}" "${RUN_OUTPUT}" PARENT_SCOPE)
  endif(ARGN)

  show_result(${RESULT_VAR})
endmacro(check_run RESULT_VAR SOURCE)

if(MINGW OR WIN32 OR WIN64 OR MSVC)
  check_library_exists(wsock32 select "" HAVE_WSOCK32_LIB)
  if(HAVE_WSOCK32_LIB)
    set(LIBMSWSOCK wsock32)
  else(HAVE_WSOCK32_LIB)
    check_library_exists(mswsock select "" HAVE_MSWSOCK_LIB)
    if(HAVE_MSWSOCK_LIB)
      set(LIBMSWSOCK mswsock)
    else(HAVE_MSWSOCK_LIB)
      check_library_exists(ws2_32 select "" HAVE_WS2_32_LIB)
      if(HAVE_WS2_32_LIB)
        set(LIBMSWSOCK ws2_32)
      endif(HAVE_WS2_32_LIB)

    endif(HAVE_MSWSOCK_LIB)
  endif(HAVE_WSOCK32_LIB)

  check_library_exists(iphlpapi if_nametoindex "" HAVE_IPHLPAPI_LIB)
  if(HAVE_IPHLPAPI_LIB)
    set(LIBIPHLPAPI iphlpapi)
  endif(HAVE_IPHLPAPI_LIB)
endif(MINGW OR WIN32 OR WIN64 OR MSVC)

check_compile(LIBC_HAS_IP6 "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\n  struct sockaddr_in6 sa;\n  sa.sin6_family = PF_INET6;\n  (void)sa;\n  return 0;\n}\n")
check_compile(LIBC_HAS_SCOPE_ID "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\n  struct sockaddr_in6 sa;\n  sa.sin6_family = PF_INET6;\n  sa.sin6_scope_id = 23;\n  (void)sa;\n  return 0;\n}\n")
check_compile(HAVE_SYSCALL "#define _GNU_SOURCE\n#include <unistd.h>\n#include <sys/syscall.h>\n\nstatic inline ssize_t\nsys_write(int fd, const void* buf, size_t n) {\n  return syscall(SYS_write, fd, buf, n);\n}\n\nint main() { return sys_write(1, \"test\\\\n\", 5);\n}\n")

if(NOT USE_SELECT)
  check_compile(
    SUPPORT_DEVPOLL
    "#include <stdio.h>\n#include <stdlib.h>\n#include <poll.h>\n#include <netdb.h>\n#include <unistd.h>\n#include <fcntl.h>\n#include <strings.h>\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/errno.h>\n#include <netinet/in.h>\n#include <sys/devpoll.h>\n\nmain() {\n  int fd = open(\"/dev/poll\", O_RDWR);\n  struct pollfd p[100];\n  int i, r;\n  dvpoll_t timeout;\n  p[0].fd = 0;\n  p[0].events = POLLIN;\n  write(fd, p, sizeof(struct pollfd));\n  timeout.dp_timeout = 100; /* milliseconds? */\n  timeout.dp_nfds = 1;\n  timeout.dp_fds = p;\n  r = ioctl(fd, DP_POLL, &timeout);\n  for(i = 0; i < r; ++i) printf(\"event %d on fd #%d\\\\n\", p[i].revents, p[i].fd);\n}\n"
  )

  check_compile(
    SUPPORT_EPOLL
    "#include <inttypes.h>\n#include <sys/epoll.h>\n#include <stdio.h>\n\nint main() { int efd = epoll_create(10);\n  struct epoll_event x;\n  if(efd == -1)\n    return 111;\n  x.events = EPOLLIN;\n  x.data.fd = 0;\n  if(epoll_ctl(efd, EPOLL_CTL_ADD, 0 /* fd */, &x) == -1)\n    return 111;\n  {\n    int n;\n    struct epoll_event y[100];\n    if((n = epoll_wait(efd, y, 100, 1000)) == -1)\n      return 111;\n    if(n > 0)\n      printf(\"event %d on fd #%d\\\\n\", y[0].events, y[0].data.fd);\n  }\n  return 0;\n}\n"
  )
  check_compile(
    SUPPORT_KQUEUE
    "#include <sys/types.h>\n#include <sys/event.h>\n#include <sys/time.h>\n\nint main() { int kq = kqueue();\n  struct kevent kev;\n  struct timespec ts;\n  if(kq == -1)\n    return 111;\n  EV_SET(&kev, 0 /* fd */, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);\n  ts.tv_sec = 0;\n  ts.tv_nsec = 0;\n  if(kevent(kq, &kev, 1, 0, 0, &ts) == -1)\n    return 111;\n\n  {\n    struct kevent events[100];\n    int i, n;\n    ts.tv_sec = 1;\n    ts.tv_nsec = 0;\n    switch(n = kevent(kq, 0, 0, events, 100, &ts)) {\n      case -1: return 111;\n      case 0: puts(\"no data on fd #0\"); break;\n    }\n    for(i = 0; i < n; ++i) {\n      printf(\"ident %d, filter %d, flags %d, fflags %d, data %d\\\\n\",\n             events[i].ident,\n             events[i].filter,\n             events[i].flags,\n             events[i].fflags,\n             events[i].data);\n    }\n  }\n  return 0;\n}\n"
  )

  check_compile(
    SUPPORT_SIGIO
    "#define _GNU_SOURCE\n#include <sys/types.h>\n#include <sys/unistd.h>\n#include <sys/poll.h>\n#include <signal.h>\n#include <fcntl.h>\n#include <stdio.h>\n\nint main() { int signum = SIGRTMIN + 1;\n  sigset_t ss;\n  sigemptyset(&ss);\n  sigaddset(&ss, signum);\n  sigaddset(&ss, SIGIO);\n  sigprocmask(SIG_BLOCK, &ss, 0);\n\n  fcntl(0 /* fd */, F_SETOWN, getpid());\n  fcntl(0 /* fd */, F_SETSIG, signum);\n#if defined(O_ONESIGFD) && defined(F_SETAUXFL)\n  fcntl(0 /* fd */, F_SETAUXFL, O_ONESIGFD);\n#endif\n  fcntl(0 /* fd */, F_SETFL, fcntl(0 /* fd */, F_GETFL) | O_NONBLOCK | O_ASYNC);\n\n  {\n    siginfo_t info;\n    struct timespec timeout;\n    int r;\n    timeout.tv_sec = 1;\n    timeout.tv_nsec = 0;\n    switch((r = sigtimedwait(&ss, &info, &timeout))) {\n      case SIGIO:\n        /* signal queue overflow */\n        signal(signum, SIG_DFL);\n        /* do poll */\n        break;\n      default:\n        if(r == signum) {\n          printf(\"event %c%c on fd #%d\\\\n\",\n                 info.si_band & POLLIN ? 'r' : '-',\n                 info.si_band & POLLOUT ? 'w' : '-',\n                 info.si_fd);\n        }\n    }\n  }\n  return 0;\n}\n"
  )
  check_compile(
    SUPPORT_POLL
    "#include <sys/types.h>\n#include <fcntl.h>\n#include <unistd.h>\n#include <poll.h>\n\nint main() { struct pollfd x;\n\n  x.fd = open(\"trypoll.c\", O_RDONLY);\n  if(x.fd == -1)\n    _exit(111);\n  x.events = POLLIN;\n  if(poll(&x, 1, 10) == -1)\n    _exit(1);\n  if(x.revents != POLLIN)\n    _exit(1);\n\n  /* XXX: try to detect and avoid poll() imitation libraries */\n\n  _exit(0);\n}\n")
  check_compile(SUPPORT_SELECT "#include <sys/types.h>\n#include <sys/time.h>\n#if defined(_WIN32) || defined(__MINGW32__)\n#include <winsock2.h>\n#else\n#include <sys/select.h>\n#include <unistd.h>\n#endif\n\nint\nmain() {\n  struct timeval tv;\n  fd_set rfds;\n  int fd = 0;\n  FD_ZERO(&rfds);\n  FD_SET(fd, &rfds);\n  tv.tv_sec = tv.tv_usec = 0;\n  select(fd + 1, &rfds, 0, 0, &tv);\n}\n"
                ${LIBMSWSOCK})
endif(NOT USE_SELECT)

check_compile(HAVE_N2I "#include <sys/types.h>\n#include <sys/socket.h>\n#include <net/if.h>\n\nint main() { static char ifname[IFNAMSIZ];\n  char* tmp = if_indextoname(0, ifname);\n  (void)tmp;\n  return 0;\n}\n")

check_compile(
  HAVE_BSDSENDFILE
  "/* for macos X, don't ask */\n#define SENDFILE 1\n\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/uio.h>\n#include <errno.h>\n#include <netinet/in.h>\n#include <netinet/tcp.h>\n\nint main() { struct sf_hdtr hdr;\n  struct iovec v[17 + 23];\n  int r, fd = 1;\n  off_t sbytes;\n  hdr.headers = v;\n  hdr.hdr_cnt = 17;\n  hdr.trailers = v + 17;\n  hdr.trl_cnt = 23;\n  r = sendfile(0, 1, 37, 42, &hdr, &sbytes, 0);\n  (void)r;\n  return 0;\n}\n"
)

check_compile(
  HAVE_SENDFILE
  "#ifdef __hpux__\n#include <sys/types.h>\n#include <sys/uio.h>\n#include <sys/socket.h>\n#include <stdio.h>\n\nint main() { /*\n        sbsize_t sendfile(int s, int fd, off_t offset, bsize_t nbytes,\n                      const struct iovec *hdtrl, int flags);\n  */\n  struct iovec x[2];\n  int fd = open(\"havesendfile.c\", 0);\n  x[0].iov_base = \"header\";\n  x[0].iov_len = 6;\n  x[1].iov_base = \"footer\";\n  x[1].iov_len = 6;\n  sendfile(1 /* dest socket */, fd /* src file */, 0 /* offset */, 23 /* nbytes */, x, 0);\n  perror(\"sendfile\");\n  return 0;\n}\n#elif defined(__sun__) && defined(__svr4__)\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/sendfile.h>\n#include <stdio.h>\n\nint main() { off_t o;\n  o = 0;\n  sendfile(1 /* dest */, 0 /* src */, &o, 23 /* nbytes */);\n  perror(\"sendfile\");\n  return 0;\n}\n#elif defined(_AIX)\n\n#define _FILE_OFFSET_BITS 64\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <unistd.h>\n#include <stdio.h>\n\nint main() { int fd = open(\"havesendfile.c\", 0);\n  struct sf_parms p;\n  int destfd = 1;\n  p.header_data = \"header\";\n  p.header_length = 6;\n  p.file_descriptor = fd;\n  p.file_offset = 0;\n  p.file_bytes = 23;\n  p.trailer_data = \"footer\";\n  p.trailer_length = 6;\n  if(send_file(&destfd, &p, 0) >= 0)\n    printf(\"sent %lu bytes.\\\\n\", p.bytes_sent);\n  return 0;\n}\n#elif defined(__linux__)\n\n#define _FILE_OFFSET_BITS 64\n#include <sys/types.h>\n#include <unistd.h>\n#if defined(__GLIBC__)\n#include <sys/sendfile.h>\n#elif defined(__dietlibc__)\n#include <sys/sendfile.h>\n#else\n#include <linux/unistd.h>\n_syscall4(int, sendfile, int, out, int, in, long*, offset, unsigned long, count)\n#endif\n#include <stdio.h>\n\nint main() { int fd = open(\"havesendfile.c\", 0);\n  off_t o = 0;\n  off_t r = sendfile(1, fd, &o, 23);\n  if(r != -1)\n    printf(\"sent %llu bytes.\\\\n\", r);\n  return 0;\n}\n\n#else\n#error unsupported architecture\n/* stupid fucking IRIX c99 does not signal #error via a non-zero exit\n * code!  ARGH!  So insert a parse error */\n#include rumpelstilzchen\n)\n#endif\n"
)

check_compile(HAVE_SOCKLEN_T "#include <sys/types.h>\n#ifdef __MINGW32__\n#include <winsock2.h>\n#include <ws2tcpip.h>\n#else\n#include <sys/socket.h>\n#include <netinet/in.h>\n#endif\n\nint main() { socklen_t t;\n  (void)t;\n  return 0;\n}\n")
if(NOT HAVE_SOCKLEN_T)
  add_definitions(-Dsocklen_t=int)
endif(NOT HAVE_SOCKLEN_T)

check_compile(HAVE___INLINE__ "static __inline__ int\nfoo(int bar) {\n  return bar + 1;\n}\nint main() { return foo(42); }\n")
if(HAVE___INLINE__)
  add_definitions(-Dinline=__inline)
else(HAVE___INLINE__)
  check_compile(HAVE___INLINE "static __inline int\nfoo(int bar) {\n  return bar + 1;\n}\nint main() { return foo(42); }\n")
  if(HAVE___INLINE)
    add_definitions(-Dinline=__inline)
  else(HAVE___INLINE)
    check_compile(HAVE_INLINE "static inline int\nfoo(int bar) {\n  return bar + 1;\n}\nint main() { return foo(42); }\n")
    if(NOT HAVE_INLINE)
      add_definitions(-Dinline=)
    endif(NOT HAVE_INLINE)
  endif(HAVE___INLINE)
endif(HAVE___INLINE__)

check_compile(HAVE_UINT128 "int main() { __uint128_t tmp; }\n")
check_compile(HAVE_INT128 "int main() { __int128_t tmp; }\n")

check_compile(HAVE_ALLOCA "#include <stdlib.h>\n#ifdef A\n#include <alloca.h>\n#else\n#include <malloc.h>\n#endif\n\nint main() { char* c = alloca(23);\n  (void)c;\n  return 0;\n}\n")

check_include_file(signal.h HAVE_SIGNAL_H)
if(HAVE_SIGNAL_H)
  # add_definitions(-DHAVE_SIGNAL_H)
  set(CMAKE_EXTRA_INCLUDE_FILES signal.h ${CMAKE_EXTRA_INCLUDE_FILES})
endif(HAVE_SIGNAL_H)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
if(HAVE_SYS_TYPES_H)
  # add_definitions(-DHAVE_SYS_TYPES_H)
  set(CMAKE_EXTRA_INCLUDE_FILES sys/types.h ${CMAKE_EXTRA_INCLUDE_FILES})
endif(HAVE_SYS_TYPES_H)

check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
  # add_definitions(-DHAVE_UNISTD_H)
  set(CMAKE_EXTRA_INCLUDE_FILES unistd.h ${CMAKE_EXTRA_INCLUDE_FILES})
endif(HAVE_UNISTD_H)

message(STATUS "extra includes = ${CMAKE_EXTRA_INCLUDE_FILES}")

if(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CROSS FALSE)
else(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CROSS TRUE)
endif(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_TOOLCHAIN_FILE)

if(NOT CROSS)
  check_type_size(sigset_t SIGSET_T_SIZE)
endif(NOT CROSS)
# message("CROSS = ${CROSS}")

# check_symbol_exists(__sync_val_compare_and_swap "" HAVE___SYNC_VAL_COMPARE_AND_SWAP)

check_compile(INTRINSIC_CAS "#include <stdlib.h>\n\nint main() {\n  long v;\n  __sync_val_compare_and_swap(&v, 1, 0);\n  return 0;\n}")

if(NOT INTRINSIC_CAS)
  add_definitions(-DUSE_INLINE_COMPARE_AND_SWAP=1)
endif()

# DUMP(CMAKE_C_COMPILER_ID)

if(CMAKE_C_COMPILER_ID STREQUAL TinyCC)
  set(COMPILER_TCC TRUE)
else(CMAKE_C_COMPILER_ID STREQUAL TinyCC)
  check_symbol_exists(__TINYC__ "" COMPILER_TCC)
endif(CMAKE_C_COMPILER_ID STREQUAL TinyCC)

if(COMPILER_TCC)
  message(STATUS "Compiler is TinyCC")
  add_definitions(-DTCC=1)

  unset(CMAKE_ASM_COMPILER)
  set(CMAKE_ASM_COMPILER cc)
endif(COMPILER_TCC)
# message("sigset_t size = ${SIGSET_T_SIZE}")
if(SIGSET_T_SIZE)
  set(HAVE_SIGSET_TYPE TRUE CACHE BOOL "Define this if you have the sigset_t type")
else(SIGSET_T_SIZE)
  check_symbol_exists(sigset_t "signal.h" HAVE_SIGSET_T)
endif(SIGSET_T_SIZE)

if(NOT HAVE_SIGSET_T OR NOT SIZEOF_SIGSET_T)

  check_run(HAVE_SIGSET_T "#define _GNU_SOURCE\n#include <sys/types.h>\n#include <signal.h>\n#include <fcntl.h>\n#include <stdio.h>\n\nint main() {\n  sigset_t ss;\nprintf(\"%lu\\\\n\", sizeof(ss));\n  return 0;\n}\n" SIZEOF_SIGSET_T)
  # message("SIZEOF_SIGSET_T = ${SIZEOF_SIGSET_T}")
endif(NOT HAVE_SIGSET_T OR NOT SIZEOF_SIGSET_T)

if(HAVE_SIGSET_T)
  # add_definitions(-DHAVE_SIGSET_T)
endif(HAVE_SIGSET_T)

# set(SIZEOF_SIGSET_TYPE "${SIZEOF_SIGSET_T}" CACHE STRING "Size of 'sigset_t' type")
message(STATUS "size of sigset_t = ${SIZEOF_SIGSET_T}")

if(SIZEOF_SIGSET_T)
  add_definitions(-DSIZEOF_SIGSET_T=${SIZEOF_SIGSET_T})
endif(SIZEOF_SIGSET_T)

check_include_file(alloca.h HAVE_ALLOCA_H)
if(HAVE_ALLOCA_H)
  # add_definitions(-DHAVE_ALLOCA_H)
endif(HAVE_ALLOCA_H)

check_include_file(errno.h HAVE_ERRNO_H)
if(HAVE_ERRNO_H)
  add_definitions(-DHAVE_ERRNO_H)
endif(HAVE_ERRNO_H)

check_include_file(sys/wait.h HAVE_SYS_WAIT_H)
if(HAVE_SYS_WAIT_H)
  add_definitions(-DHAVE_SYS_WAIT_H)
endif(HAVE_SYS_WAIT_H)

if(NOT CROSS)
  if(BUILD_LZMA)
    check_type_size(int16_t INT16_T)
    check_type_size(int32_t INT32_T)
    check_type_size(int64_t INT64_T)
    check_type_size(intmax_t INTMAX_T)
    check_type_size(uint8_t UINT8_T)
    check_type_size(uint16_t UINT16_T)
    check_type_size(uint32_t UINT32_T)
    check_type_size(uint64_t UINT64_T)
    check_type_size(uintmax_t UINTMAX_T)

    #
    check_type_size(ptrdiff_t PTRDIFF_T_SIZE)
    check_type_size(size_t SIZE_T_SIZE)
    check_type_size(long LONG_SIZE)
    check_type_size("void*" VOIDP_T_SIZE)

    # DUMP(PTRDIFF_T_SIZE) DUMP(SIZE_T_SIZE) DUMP(LONG_SIZE) DUMP(VOIDP_T_SIZE)

    #
    check_type_size("short" SIZE_OF_SHORT)
    check_type_size("int" SIZE_OF_INT)
    check_type_size("long" SIZE_OF_LONG)
    check_type_size("long long" SIZE_OF_LONG_LONG)

    #
    check_type_size("unsigned short" SIZE_OF_UNSIGNED_SHORT)
    check_type_size("unsigned" SIZE_OF_UNSIGNED)
    check_type_size("unsigned long" SIZE_OF_UNSIGNED_LONG)
    check_type_size("unsigned long long" SIZE_OF_UNSIGNED_LONG_LONG)
    check_type_size("size_t" SIZE_OF_SIZE_T)
  endif(BUILD_LZMA)
  #
  check_type_size("__int64" __INT64)
  check_type_size("unsigned __int64" UNSIGNED___INT64)

  check_type_size(uintptr_t UINTPTR_T)
  if(NOT HAVE_UINTPTR_T)
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
      set(uintptr_t "uint64_t")
    else()
      set(uintptr_t "uint32_t")
    endif()
  endif()
endif(NOT CROSS)

string(REGEX REPLACE ".*/" "" COMPILER_NAME "${CMAKE_C_COMPILER}")

if(COMPILER_NAME MATCHES "em.*")
  set(EMSCRIPTEN TRUE)
  dump(EMSCRIPTEN EMCC_SUFFIX EMSCRIPTEN_PREFIX EMSCRIPTEN_TOOLCHAIN_PATH COMPILER_NAME CMAKE_EXE_LINKER_FLAGS)
  # foreach(LIB BZIP ZLIB LZMA SSL) set("USE_${LIB}" OFF) endforeach(LIB BZIP ZLIB LZMA SSL)
  set(EMSCRIPTEN_EXE_SUFFIX "html")
endif(COMPILER_NAME MATCHES "em.*")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  check_c_compiler_flag("-O0" O_OPT_NONE)
  if(O_OPT_NONE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
  endif(O_OPT_NONE)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

check_c_compiler_flag("-flto" F_LTO)
check_c_compiler_flag("-fPIC" F_PIC)
check_c_compiler_flag("-fPIE" F_PIE)

foreach(FLAG lto PIC PIE)
  check_c_compiler_flag("-f${FLAG}" "F_${FLAG}")
  string(TOUPPER "${FLAG}" NAME)

  if("${F_${NAME}}")
    set("${F_${NAME}}" "-f${FLAG}")
  else("${F_${NAME}}")
    set("${F_${NAME}}" "")
  endif("${F_${NAME}}")
endforeach()

if(F_LTO)
  option(ENABLE_LTO "Enable link-time optimization" OFF)
endif(F_LTO)
if(EMSCRIPTEN)
  option(ENABLE_LTO "Enable link-time optimization" ON)
endif(EMSCRIPTEN)

#[[
if(ENABLE_LTO)
  set(F_LTO "-flto")
else(ENABLE_LTO)
  set(F_LTO "")
endif(ENABLE_LTO)
]]

if(ENABLE_LTO)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
endif(ENABLE_LTO)

if(LINK_STATIC)
  set(EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS} -static")
  set(CHK_DLFCN FALSE)
else(LINK_STATIC)
  set(EXE_LINKER_FLAGS "-Wl,-rpath=${OPENSSL_LIB_DIR}"=)
  set(CHK_DLFCN TRUE)
endif(LINK_STATIC)

set(CMAKE_STATIC_LINKER_FLAGS "")

macro(check_function_def FUNC)
  if(ARGC EQUAL 1)
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
  else(ARGC EQUAL 1)
    set(RESULT_VAR "${ARGN}")
  endif(ARGC EQUAL 1)
  # message("FUNC: ${FUNC} RESULT_VAR: ${RESULT_VAR}")
  check_function_exists("${FUNC}" "${RESULT_VAR}")
  if(${${RESULT_VAR}})
    # add_definitions(-D${RESULT_VAR}) message("${RESULT_VAR} ${${RESULT_VAR}}")
  endif(${${RESULT_VAR}})
endmacro(check_function_def FUNC RESULT_VAR)
function(check_functions)
  foreach(FUNC ${ARGN})
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
    check_function_def("${FUNC}" "${RESULT_VAR}")
  endforeach(FUNC ${ARGN})
endfunction(check_functions)

check_functions(lstat waitpid aligned_alloc fnmatch getdelim getopt getopt_long pipe2 popen posix_memalign pread sendfile sigaddset sigemptyset sigprocmask strerror_r __write writev posix_spawnattr_setflags posix_spawn_file_actions_init posix_spawn_file_actions_adddup2 posix_spawnp clock_gettime)
if(HAVE_POSIX_SPAWNATTR_SETFLAGS AND HAVE_POSIX_SPAWN_FILE_ACTIONS_INIT AND HAVE_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2 AND HAVE_POSIX_SPAWNP)
  add_definitions(-DPOSIX_SPAWN=1)
endif()

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
  check_function_exists(poll_wait SUPPORT_POLL)
  if(SUPPORT_POLL)
    message(INFO "Checking for poll()... OK")
    set(USE_POLL TRUE)
    set(USE_SELECT FALSE)
  else(SUPPORT_POLL)
    message(INFO "Checking for poll()... NOT FOUND")
    set(USE_POLL TRUE)
  endif(SUPPORT_POLL)
endif(USE_POLL STREQUAL "OFF" AND NOT USE_SELECT STREQUAL "ON")

check_function_exists(daemon HAVE_DAEMON)

if(HAVE_DAEMON)
  add_definitions(-DHAVE_DAEMON=1)
endif(HAVE_DAEMON)
check_function_exists(opendir HAVE_OPENDIR)
check_function_exists(readdir HAVE_READDIR)
check_function_exists(closedir HAVE_CLOSEDIR)

if(HAVE_OPENDIR AND HAVE_READDIR AND HAVE_CLOSEDIR)
  add_definitions(-DUSE_READDIR=1)
endif(HAVE_OPENDIR AND HAVE_READDIR AND HAVE_CLOSEDIR)

check_function_exists(wordexp HAVE_WORDEXP)

if(HAVE_WORDEXP)
  add_definitions(-DHAVE_WORDEXP=1)
endif(HAVE_WORDEXP)
#[[
#message("USE_SELECT=${USE_SELECT}")
#message("USE_POLL=${USE_POLL}")
message("USE_EPOLL=${USE_EPOLL}")]]
#[[if(USE_POLL STREQUAL "OFF"
   AND USE_SELECT STREQUAL "OFF"
   AND USE_EPOLL STREQUAL "OFF")
  set(USE_SELECT TRUE)
endif()
if(USE_POLL STREQUAL "ON")
  add_definitions(-DHAVE_POLL=1 -DIOPAUSE_POLL=1)
endif()

if(USE_SELECT STREQUAL "ON")
  add_definitions(-DUSE_SELECT=1 -DIOPAUSE_SELECT=1)
endif()
]]
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(stdbool.h HAVE_STDBOOL_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(sys/param.h HAVE_SYS_PARAM_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)

foreach(DEF HAVE_STDINT_H HAVE_STDBOOL_H HAVE_INTTYPES_H)
  if(${${DEF}})
    set(DEFINES_H "${DEFINES_H} -D${DEF}=1")
  endif()
endforeach()
# message(DEFINES_H: ${DEFINES_H}) add_definitions(${DEFINES_H})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEFINES_H} -DHAVE_CONFIG_H=1")

# message(INFO "Checking for epoll()...") check_function_exists(epoll_wait SUPPORT_EPOLL) if(SUPPORT_EPOLL) message(INFO "Checking for epoll()... OK") set(USE_EPOLL ON) set(USE_SELECT OFF) else(SUPPORT_EPOLL) message(INFO "Checking for epoll()... NOT FOUND") set(USE_EPOLL OFF) endif(SUPPORT_EPOLL)

# if(USE_EPOLL STREQUAL ON) add_definitions(-DHAVE_EPOLL=1) endif()

check_library_exists(compat syscall "" HAVE_COMPAT_LIB)
if(HAVE_COMPAT_LIB)
  set(LIBCOMPAT compat)
endif()

check_library_exists(m roundf "" HAVE_MATH_LIB)
if(HAVE_MATH_LIB)
  set(LIBMATH m)
endif()

if(NOT LINK_STATIC)
  check_library_exists(dl dlopen "" HAVE_DLFCN_LIB)
  if(HAVE_DLFCN_LIB)
    set(LIBDL dl)
  endif()
endif(NOT LINK_STATIC)

# include(${CMAKE_SOURCE_DIR}/FindZlib.cmake) include(${CMAKE_SOURCE_DIR}/FindBZip2.cmake) include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)

include(${CMAKE_SOURCE_DIR}/build/cmake/CheckInline.cmake)
include(${CMAKE_SOURCE_DIR}/build/cmake/Profile.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  debug_option(OUTPUT "Output verbose messages")
  add_definitions(-D_DEBUG=1)

  if(DEBUG_OUTPUT)
    add_definitions(-DDEBUG_OUTPUT=1)
  endif(DEBUG_OUTPUT)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_MINSIZEREL)
foreach(CompilerFlag ${CompilerFlags})
  unset(FLAG)
  string(REPLACE "/MD" "/MT" FLAG "${${CompilerFlag}}")
  # string(REPLACE "/MTd" "/MT" FLAG "${FLAG}")
  if(${CompilerFlag} MATCHES "DEBUG")
    set(FLAG "${FLAG} -D_DEBUG=1")
  endif()
  string(REPLACE " " ";" FLAG "${FLAG}")
  list(REMOVE_DUPLICATES FLAG)
  string(REPLACE ";" " " FLAG "${FLAG}")
  unset(${CompilerFlag} CACHE)
  set(${CompilerFlag} "${FLAG}" CACHE STRING "Compiler flags ${CompilerFlag}")
  # message("${CompilerFlag}: ${${CompilerFlag}}")
endforeach()

include(${CMAKE_SOURCE_DIR}/build/cmake/Werror.cmake)

if(USE_LZMA)
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # Check LibLZMA
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # pkg_search_module(LIBLZMA liblzma lzma) if(LIBLZMA_FOUND) if(pkgcfg_lib_LIBLZMA_lzma) set(LIBLZMA_LIBRARY ${pkgcfg_lib_LIBLZMA_lzma} CACHE FILEPATH "") endif(pkgcfg_lib_LIBLZMA_lzma) endif(LIBLZMA_FOUND)
  #
  # if(NOT LIBLZMA_FOUND) include(FindLibLZMA) endif(NOT LIBLZMA_FOUND)

  if(NOT LIBLZMA_FOUND)
    check_library_exists(lzma lzma_auto_decoder "" HAVE_LIBLZMA)
    check_include_file(lzma.h HAVE_LZMA_H)

    if(HAVE_LIBLZMA AND HAVE_LZMA_H)
      set(LIBLZMA_FOUND TRUE)
      set(LIBLZMA_INCLUDE_DIRS "")
      set(LIBLZMA_LIBRARY lzma)
      message(STATUS "Found liblzma")
      message(STATUS "  LIBLZMA_LIBRARY=${LIBLZMA_LIBRARY}")
    endif(HAVE_LIBLZMA AND HAVE_LZMA_H)

  endif(NOT LIBLZMA_FOUND)

else(USE_LZMA)
  unset(LIBLZMA_LIBRARY CACHE)
  unset(LIBLZMA_INCLUDE_DIRS CACHE)
  unset(LIBLZMA_FOUND CACHE)

endif(USE_LZMA)

if(USE_ZLIB)

  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # Check Zlib
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  message(STATUS "Checking for libz")
  find_library(ZLIB z)

  message(STATUS "${ZLIB}")

  set(ZLIB_LIBRARY "${ZLIB}")

  if(ZLIB)
    set(ZLIB_FOUND TRUE)
    if(ZLIB STREQUAL "")
      set(ZLIB z)
    endif(ZLIB STREQUAL "")
    set(ZLIB_LIBARY_RELEASE "${ZLIB}" CACHE FILEPATH "")
    set(ZLIB_LIBARY_DEBUG "${ZLIB}" CACHE FILEPATH "")
    set(ZLIB_LIBARY "${ZLIB}" CACHE FILEPATH "")
  endif(ZLIB)

  if(NOT ZLIB_FOUND)

    pkg_search_module(ZLIB zlib libz z)
    if(ZLIB_FOUND)
      if(pkgcfg_lib_ZLIB_z)
        set(ZLIB_LIBRARY_RELEASE "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
        set(ZLIB_LIBRARY_DEBUG "${pkgcfg_lib_ZLIB_z}" CACHE FILEPATH "")
      endif(pkgcfg_lib_ZLIB_z)
    else(ZLIB_FOUND)
      # include(FindZlib)
    endif(ZLIB_FOUND)

  endif(NOT ZLIB_FOUND)

else(USE_ZLIB)
  unset(ZLIB_LIBRARY_RELEASE CACHE)
  unset(ZLIB_LIBRARY_DEBUG CACHE)
  unset(ZLIB_LIBRARY CACHE)
endif(USE_ZLIB)

if(USE_BZIP)
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  # Check BZip2
  # ############################################################################################################################################################################################################################################################################################################################################################################################################
  pkg_search_module(BZIP2 libbz2 bzip2)
  if(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    message(STATUS "bzip2 package config not found")
    #[[  include(FindBZip2)

    if(BZIP2_FOUND)
      set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
      set(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIRS})
    endif(BZIP2_FOUND)]]
    find_library(BZIP2_LIBRARY NAMES bz2)
    if(BZIP2_LIBRARY)
      message(STATUS "bzip2 library found: ${BZIP2_LIBRARY}")
      set(BZIP2_FOUND TRUE)
    endif(BZIP2_LIBRARY)

  else(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    # pkg_get_variable(BZIP2_LIBRARY "${BZIP2_MODULE_NAME}")
    if(pkgcfg_lib_BZIP2_bz2)
      set(BZIP2_LIBRARY ${pkgcfg_lib_BZIP2_bz2})
    else(pkgcfg_lib_BZIP2_bz2)
      if(BZIP2_LIBRARIES)
        set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
      else(BZIP2_LIBRARIES)
        set(BZIP2_LIBRARY bz2)
      endif(BZIP2_LIBRARIES)
    endif(pkgcfg_lib_BZIP2_bz2)
    set(BZIP2_LINK_LIBRARY ${pkgcfg_lib_BZIP2_bz2} ${BZIP2_LINK_LIBRARIES})
    set(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIRS})
    set(BZIP2_LIBRARY_DIR ${BZIP2_LIBRARY_DIRS})
    message(STATUS "bzip2 package found: ${BZIP2_INCLUDE_DIR} ${BZIP2_LIBRARY}")
  endif(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")

  unset(BZIP2_LDFLAGS CACHE)
  unset(BZIP2_STATIC_LDFLAGS CACHE)
endif(USE_BZIP)

if(USE_BROTLI)
  pkg_search_module(BROTLIDEC libbrotlidec brotlidec)
  pkg_search_module(BROTLIENC libbrotlienc brotlienc)
  pkg_search_module(BROTLICOMMON libbrotlicommon brotlicommon)
  if(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")
    message(STATUS "brotli package config not found")

    find_library(BROTLIDEC_LIBRARY NAMES brotlidec)
    find_library(BROTLIENC_LIBRARY NAMES brotlienc)
    find_library(BROTLICOMMON_LIBRARY NAMES brotlicommon)
    if(BROTLIDEC_LIBRARY AND BROTLIENC_LIBRARY AND BROTLICOMMON_LIBRARY)
      message(STATUS "brotli library found: ${BROTLIDEC_LIBRARY} ${BROTLIENC_LIBRARY} ${BROTLICOMMON_LIBRARY}")
      set(BROTLI_FOUND TRUE)
      set(BROTLI_LIBRARY ${BROTLIDEC_LIBRARY} ${BROTLIENC_LIBRARY} ${BROTLICOMMON_LIBRARY})

    endif(BROTLIDEC_LIBRARY AND BROTLIENC_LIBRARY AND BROTLICOMMON_LIBRARY)

  else(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")
    message(STATUS "brotli package found: ${BROTLI_PREFIX} ${BROTLI_LIBRARY_DIRS} ${BROTLI_LIBRARIES}")
    # pkg_get_variable(BROTLI_LIBRARY "${BROTLI_MODULE_NAME}")
    set(BROTLI_LIBRARY ${pkgcfg_lib_BROTLIENC_brotlienc} ${pkgcfg_lib_BROTLIDEC_brotlidec} ${pkgcfg_lib_BROTLICOMMON_brotlicommon})
    set(BROTLI_LINK_LIBRARY ${pkgcfg_lib_BROTLIENC_brotlienc} ${pkgcfg_lib_BROTLIDEC_brotlidec} ${pkgcfg_lib_BROTLICOMMON_brotlicommon})
    set(BROTLI_INCLUDE_DIR ${BROTLICOMMON_INCLUDE_DIRS})
    set(BROTLI_LIBRARY_DIR ${BROTLICOMMON_LIBRARY_DIRS})
    set(BROTLI_FOUND TRUE)
  endif(NOT BROTLICOMMON_FOUND OR "${BROTLICOMMON_FOUND}" STREQUAL "")

endif(USE_BROTLI)

# ##############################################################################################################################################################################################################################################################################################################################################################################################################
# Check OpenSSL
# ##############################################################################################################################################################################################################################################################################################################################################################################################################
if(USE_SSL AND NOT BUILD_SSL)
  message("USE_SSL = ${USE_SSL}")
  if(NOT OPENSSL_ROOT_DIR)
    if("$ENV{OPENSSL_ROOT}")
      set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT}" CACHE FILEPATH "OpenSSL root directory")
   # else("$ENV{OPENSSL_ROOT}")
   #   set(OPENSSL_ROOT_DIR "${CMAKE_INSTALL_PREFIX}" CACHE FILEPATH "OpenSSL root directory")
    endif("$ENV{OPENSSL_ROOT}")
  endif(NOT OPENSSL_ROOT_DIR)

  #set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH OFF)
    message(STATUS "Searching pkgconfig database for openssl...")
    pkg_search_module(OPENSSL openssl libssl)

  if(OPENSSL_FOUND)
    check_include_file(openssl/ssl.h HAVE_OPENSSL_SSL_H)
    if(pkgcfg_lib_OPENSSL_crypto AND NOT OPENSSL_CRYPTO_LIBRARY)
      set(OPENSSL_CRYPTO_LIBRARY "${pkgcfg_lib_OPENSSL_crypto}")
      list(APPEND OPENSSL_LIBRARIES ${pkgcfg_lib_OPENSSL_crypto})
    endif(pkgcfg_lib_OPENSSL_cryptoAND NOT OPENSSL_CRYPTO_LIBRARY)
    if(pkgcfg_lib_OPENSSL_ssl)

      if(pkgcfg_lib_OPENSSL_ssl)
        if(NOT OPENSSL_SSL_LIBRARY)
          set(OPENSSL_SSL_LIBRARY "${pkgcfg_lib_OPENSSL_ssl}")
        endif(NOT OPENSSL_SSL_LIBRARY)
      else(pkgcfg_lib_OPENSSL_ssl)
        string(REPLACE "libcrypto" "libssl" OPENSSL_SSL_LIB "${OPENSSL_CRYPTO_LIBRARY}")
        set(OPENSSL_SSL_LIBRARY "${OPENSSL_SSL_LIB}" CACHE STRING "SSL library" FORCE)
      endif(pkgcfg_lib_OPENSSL_ssl)

      list(APPEND OPENSSL_LIBRARIES ${pkgcfg_lib_OPENSSL_ssl})
    endif(pkgcfg_lib_OPENSSL_ssl)
  else(OPENSSL_FOUND)
     message(STATUS "Searching for openssl using FindOpenSSL.cmake script...")
   include(FindOpenSSL)
  endif(OPENSSL_FOUND)

  if(NOT HAVE_OPENSSL_SSL_H)
    set(OPENSSL_FOUND FALSE)
  endif(NOT HAVE_OPENSSL_SSL_H)

  message(OPENSSL_FOUND: ${OPENSSL_FOUND})
  if(OPENSSL_FOUND)
    if(NOT OPENSSL_INCLUDE_DIR)
      set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    endif()
    if(NOT OPENSSL_LIB_DIR)
      set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
    endif()
  else(OPENSSL_FOUND)
    set(BUILD_SSL TRUE)
  endif(OPENSSL_FOUND)

  if(OPENSSL_SSL_LIBRARY STREQUAL "" OR NOT OPENSSL_SSL_LIBRARY)
    string(REPLACE "libcrypto" "libssl" OPENSSL_SSL_LIB "${OPENSSL_CRYPTO_LIBRARY}")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_SSL_LIB}" CACHE FILEPATH "SSL library" FORCE)
  endif(OPENSSL_SSL_LIBRARY STREQUAL "" OR NOT OPENSSL_SSL_LIBRARY)

  # message(STATUS "OPENSSL_FOUND=${OPENSSL_FOUND}\n   OPENSSL_INCLUDE_DIR=${OPENSSL_LIB_DIR}\n   OPENSSL_LIB_DIR=${OPENSSL_INCLUDE_DIR}\n   OPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}\n   OPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}\n   OPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}")

  if(OPENSSL_SSL_LIBRARY AND NOT OPENSSL_CRYPTO_LIBRARY)
    string(REPLACE "libssl" "libcrypto" OPENSSL_CRYPTO_LIBRARY "${OPENSSL_SSL_LIBRARY}")
  endif(OPENSSL_SSL_LIBRARY AND NOT OPENSSL_CRYPTO_LIBRARY)

else()
  unset(OPENSSL_LIBRARIES CACHE)
  unset(OPENSSL_CRYPTO_LIBRARY CACHE)
  unset(OPENSSL_SSL_LIBRARY CACHE)
  unset(OPENSSL_INCLUDE_DIR CACHE)
endif()
# link_libraries( ${ZLIB_LIBRARY} ${BZIP2_LIBRARY} ${LIBLZMA_LIBRARY} ${LIBSSL} ) add_definitions(-Dinline=__inline)
if(NOT OPENSSL_SSL_LIBRARY OR OPENSSL_SSL_LIBRARY STREQUAL "")

  # message("OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY}")
  string(REPLACE "libcrypto" "libssl" OPENSSL_SSL_LIBRARY "${OPENSSL_CRYPTO_LIBRARY}")
  # message("OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")
endif(NOT OPENSSL_SSL_LIBRARY OR OPENSSL_SSL_LIBRARY STREQUAL "")

set(CMAKE_REQUIRED_DEFINITIONS)
set(CMAKE_REQUIRED_INCLUDES "poll.h")
set(CMAKE_REQUIRED_FLAGS)
set(CMAKE_REQUIRED_LINK_OPTIONS)
set(CMAKE_REQUIRED_LIBRARIES)

if(NOT USE_SELECT)
  if(NOT USE_POLL)

    if(SUPPORT_POLL STREQUAL "")
      check_library_exists("" poll "" SUPPORT_POLL)
      if(NOT SUPPORT_POLL)
        check_function_exists(poll SUPPORT_POLL)
      endif(NOT SUPPORT_POLL)
    endif(SUPPORT_POLL STREQUAL "")

    if(SUPPORT_EPOLL STREQUAL "")
      check_library_exists("" epoll_create "" SUPPORT_EPOLL)
      if(NOT SUPPORT_EPOLL)
        check_function_exists(epoll_create SUPPORT_EPOLL)
      endif(NOT SUPPORT_EPOLL)
    endif(SUPPORT_EPOLL STREQUAL "")

    if(SUPPORT_KQUEUE STREQUAL "")
      check_library_exists("" kqueue "" SUPPORT_KQUEUE)
      if(NOT SUPPORT_KQUEUE)
        check_function_exists(kqueue SUPPORT_KQUEUE)
      endif(NOT SUPPORT_KQUEUE)
    endif(SUPPORT_KQUEUE STREQUAL "")

  endif(NOT USE_POLL)
endif(NOT USE_SELECT)

if(SUPPORT_SELECT STREQUAL "")
  check_library_exists("${LIBMSWSOCK}" select "" SUPPORT_SELECT)
  if(NOT SUPPORT_SELECT)
    check_function_exists(select SUPPORT_SELECT)
  endif(NOT SUPPORT_SELECT)
endif(SUPPORT_SELECT STREQUAL "")

function(list_contains LIST ITEM OUTPUT_VAR)
  set(RESULT FALSE)
  foreach(IT ${LIST})
    if("${IT}" STREQUAL "${ITEM}")
      set(RESULT TRUE)
    endif("${IT}" STREQUAL "${ITEM}")
  endforeach(IT ${LIST})
  set("${OUTPUT_VAR}" ${RESULT} PARENT_SCOPE)
endfunction(list_contains LIST ITEM OUTPUT_VAR)

set(MUX "")
set(MUX_DEFS "")
set(MUX_ALL
    KQUEUE DEVPOLL EPOLL
    # SIGIO
    POLL SELECT)
set(MUX_USE KQUEUE EPOLL DEVPOLL SIGIO POLL SELECT)
foreach(METHOD ${MUX_USE})
  if(SUPPORT_${METHOD})
    list_contains("${MUX_LIST}" "${METHOD}" CONTAINS_METHOD)
    if(NOT CONTAINS_METHOD)
      list(APPEND MUX_LIST ${METHOD})
    endif(NOT CONTAINS_METHOD)
    if(MUX STREQUAL "")
      set(MUX "${METHOD}")
    endif(MUX STREQUAL "")
    set(MUX_DEFS "${MUX_DEFS} -DHAVE_${METHOD}=1")
  endif(SUPPORT_${METHOD})
endforeach()
foreach(METHOD ${MUX_ALL})
  if(SUPPORT_${METHOD})
    set(MUX_DEFS "${MUX_DEFS} -DSUPPORT_${METHOD}=1")
  endif(SUPPORT_${METHOD})
endforeach()
set(MUX_METHODS "${MUX_LIST}")

message(STATUS "Available MUX methods: ${MUX_METHODS}")
string(REGEX REPLACE "[;\n ]" ";" __MUX_METHODS "${MUX_METHODS}")
unset(MUX_METHODS)
set(MUX_METHOD SELECT CACHE STRING "multiplex methods")

set_property(CACHE MUX_METHOD PROPERTY STRINGS ${__MUX_METHODS})

# message("MUX definitions: ${MUX_DEFS}")

if(LIBMSWSOCK)
  list(APPEND CMAKE_REQUIRED_LIBRARIES ${LIBMSWSOCK})
endif(LIBMSWSOCK)

set(MUX_METHOD "${MUX}" CACHE STRING "I/O Multiplexer method")
message(STATUS "MUX method: ${MUX_METHOD}")

if(MUX_METHOD STREQUAL "SELECT")
  set(io_COMPILE_FLAGS -DUSE_SELECT=1)
else()
  set(io_COMPILE_FLAGS "-DHAVE_${MUX_METHOD}=1")
endif()
add_definitions("-DHAVE_${MUX_METHOD}=1" "-DHAVE_${MUX}=1")

if(WIN32 OR MSVC)
  if(USE_WIDECHAR)
    add_definitions(-DUSE_WIDECHAR=1)
  endif(USE_WIDECHAR)
endif(WIN32 OR MSVC)

if(NOT ZLIB_FOUND OR NOT EXISTS "${ZLIB_LIBRARY}")
  unset(ZLIB_FOUND CACHE)
  unset(ZLIB_LIBRARY CACHE)
endif()

if(USE_ZLIB)
  # message("ZLIB_LIBRARY: ${ZLIB_LIBRARY} ZLIB_FOUND: ${ZLIB_FOUND}") if(ZLIB_INCLUDE_DIR) link_libraries(${ZLIB_LIBRARY})
  include_directories(${ZLIB_INCLUDE_DIR})
  # endif(ZLIB_INCLUDE_DIR)

  # if(ZLIB_LIBRARY)
  add_definitions(-DHAVE_ZLIB=1)
  # else(ZLIB_LIBRARY) add_definitions(-DHAVE_ZLIB=0) endif(ZLIB_LIBRARY)
endif(USE_ZLIB)

if(USE_BZIP)
  # message("BZIP2_LIBRARY: ${BZIP2_LIBRARY} BZIP2_FOUND: ${BZIP2_FOUND}") if(BZIP2_LIBRARY)
  add_definitions(-DHAVE_LIBBZ2=1)
  include_directories(${BZIP2_INCLUDE_DIR})
  # else(BZIP2_LIBRARY) add_definitions(-DHAVE_LIBBZ2=0) endif(BZIP2_LIBRARY)
endif(USE_BZIP)

if(USE_LZMA)
  # message("LIBLZMA_LIBRARY: ${LIBLZMA_LIBRARY} LIBLZMA_FOUND: ${LIBLZMA_FOUND}") if(LIBLZMA_INCLUDE_DIR) link_libraries(${LIBLZMA_LIBRARY})
  include_directories(${LIBLZMA_INCLUDE_DIR})
  # endif(LIBLZMA_INCLUDE_DIR) if(LIBLZMA_LIBRARY)
  add_definitions(-DHAVE_LIBLZMA=1 -DLZMA_API_STATIC=1)
  # else(LIBLZMA_LIBRARY) add_definitions(-DHAVE_LIBLZMA=0) endif(LIBLZMA_LIBRARY)
endif(USE_LZMA)

if(USE_BROTLI)
  add_definitions(-DHAVE_BROTLI=1)
  include_directories(${BROTLI_INCLUDE_DIR})
endif(USE_BROTLI)

macro(invert INVAR OUTVAR)
  if(${${INVAR}})
    set("${OUTVAR}" FALSE)
  else(${${INVAR}})
    set("${OUTVAR}" TRUE)
  endif(${${INVAR}})
endmacro(invert INVAR OUTVAR)

macro(invoption VAR DESC INVAR)
  if(${${INVAR}})
    option("${VAR}" "${DESC}" OFF)
  else(${${INVAR}})
    option("${VAR}" "${DESC}" ON)
  endif(${${INVAR}})
endmacro(invoption VAR DESC INVAR)

invoption(BUILD_LZMA "build 3rdparty liblzma" LIBLZMA_FOUND)
invoption(BUILD_BZIP2 "build 3rdparty libbz2" BZIP2_FOUND)
invoption(BUILD_ZLIB "build 3rdparty libz" ZLIB_FOUND)
invoption(BUILD_BROTLI "build 3rdparty brotli" BROTLI_FOUND)
invoption(BUILD_SSL "build 3rdparty openssl" OPENSSL_FOUND)

find_program(TAR NAMES bsdtar tar gtar gnutar star)

if(BUILD_ZLIB)
  message("Building zlib from 3rdparty/zlib")
  set(ZLIB_VERSION "1.2.11")
  set(ZLIB_DISABLE_TESTS TRUE)
  set(ZLIB_STATIC TRUE)
  set(ZLIB_NO_INSTALL TRUE)

  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/zlib/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/zlib-${ZLIB_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/zlib")
      file(DOWNLOAD "https://zlib.net/zlib-${ZLIB_VERSION}.tar.gz" "${CMAKE_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}.tar.gz")
    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/zlib-${ZLIB_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/zlib")

  endif()
  include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib.cmake")
  set(ZLIB_SOURCES 3rdparty/zlib/adler32.c 3rdparty/zlib/compress.c 3rdparty/zlib/crc32.c 3rdparty/zlib/deflate.c 3rdparty/zlib/infback.c 3rdparty/zlib/inffast.c 3rdparty/zlib/inflate.c 3rdparty/zlib/inftrees.c 3rdparty/zlib/trees.c 3rdparty/zlib/uncompr.c 3rdparty/zlib/zutil.c)
  add_library(z ${ZLIB_SOURCES})
  set_target_properties(z PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/zlib" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/zlib")
  link_directories(BEFORE "${CMAKE_BINARY_DIR}/zlib")
  set(ZLIB_LIBRARY_DEBUG z CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY_RELEASE z CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY z CACHE FILEPATH "" FORCE)
  set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/zlib CACHE PATH "" FORCE)
endif(BUILD_ZLIB)

if(BUILD_BZIP2)
  message("Building libbz2 from 3rdparty/bzip2")
  set(BZIP2_VERSION "1.0.8")
  set(BZIP2_DISABLE_TESTS TRUE)
  set(BZIP2_STATIC TRUE)
  set(BZIP2_NO_INSTALL TRUE)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/bzip2/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/bzip2-${BZIP2_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/bzip2")
      file(DOWNLOAD "https://sourceware.org/pub/bzip2/bzip2-${BZIP2_VERSION}.tar.gz" "${CMAKE_SOURCE_DIR}/3rdparty/bzip2-${BZIP2_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/bzip2-${BZIP2_VERSION}.tar.gz")

    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/bzip2-${BZIP2_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/bzip2")
    file(
      WRITE "${CMAKE_SOURCE_DIR}/3rdparty/bzip2/blocksort.c"
      "\n/*-------------------------------------------------------------*/\n/*--- Block sorting machinery                               ---*/\n/*---                                           blocksort.c ---*/\n/*-------------------------------------------------------------*/\n\n/* ------------------------------------------------------------------\n   This file is part of bzip2/libbzip2, a program and library for\n   lossless, block-sorting data compression.\n\n   bzip2/libbzip2 version 1.0.6 of 6 September 2010\n   Copyright (C) 1996-2010 Julian Seward <jseward@bzip.org>\n\n   Please read the WARNING, DISCLAIMER and PATENTS sections in the \n   README file.\n\n   This program is released under the terms of the license contained\n   in the file LICENSE.\n   ------------------------------------------------------------------ */\n\n\n#include \"bzlib_private.h\"\n\n/*---------------------------------------------*/\n/*--- Fallback O(N log(N)^2) sorting        ---*/\n/*--- algorithm, for repetitive blocks      ---*/\n/*---------------------------------------------*/\n\n/*---------------------------------------------*/\nstatic \n__inline__\nvoid fallbackSimpleSort ( UInt32* fmap, \n                          UInt32* eclass, \n                          Int32   lo, \n                          Int32   hi )\n{\n   Int32 i, j, tmp;\n   UInt32 ec_tmp;\n\n   if (lo == hi) return;\n\n   if (hi - lo > 3) {\n      for ( i = hi-4; i >= lo; i-- ) {\n         tmp = fmap[i];\n         ec_tmp = eclass[tmp];\n         for ( j = i+4; j <= hi && ec_tmp > eclass[fmap[j]]; j += 4 )\n            fmap[j-4] = fmap[j];\n         fmap[j-4] = tmp;\n      }\n   }\n\n   for ( i = hi-1; i >= lo; i-- ) {\n      tmp = fmap[i];\n      ec_tmp = eclass[tmp];\n      for ( j = i+1; j <= hi && ec_tmp > eclass[fmap[j]]; j++ )\n         fmap[j-1] = fmap[j];\n      fmap[j-1] = tmp;\n   }\n}\n\n\n/*---------------------------------------------*/\n#define fswap(zz1, zz2) \\\n   { Int32 zztmp = zz1; zz1 = zz2; zz2 = zztmp; }\n\n#define fvswap(zzp1, zzp2, zzn)       \\\n{                                     \\\n   Int32 yyp1 = (zzp1);               \\\n   Int32 yyp2 = (zzp2);               \\\n   Int32 yyn  = (zzn);                \\\n   while (yyn > 0) {                  \\\n      fswap(fmap[yyp1], fmap[yyp2]);  \\\n      yyp1++; yyp2++; yyn--;          \\\n   }                                  \\\n}\n\n\n#define fmin(a,b) ((a) < (b)) ? (a) : (b)\n\n#define fpush(lz,hz) { stackLo[sp] = lz; \\\n                       stackHi[sp] = hz; \\\n                       sp++; }\n\n#define fpop(lz,hz) { sp--;              \\\n                      lz = stackLo[sp];  \\\n                      hz = stackHi[sp]; }\n\n#define FALLBACK_QSORT_SMALL_THRESH 10\n#define FALLBACK_QSORT_STACK_SIZE   100\n\n\nstatic\nvoid fallbackQSort3 ( UInt32* fmap, \n                      UInt32* eclass,\n                      Int32   loSt, \n                      Int32   hiSt )\n{\n   Int32 unLo, unHi, ltLo, gtHi, n, m;\n   Int32 sp, lo, hi;\n   UInt32 med, r, r3;\n   Int32 stackLo[FALLBACK_QSORT_STACK_SIZE];\n   Int32 stackHi[FALLBACK_QSORT_STACK_SIZE];\n\n   r = 0;\n\n   sp = 0;\n   fpush ( loSt, hiSt );\n\n   while (sp > 0) {\n\n      AssertH ( sp < FALLBACK_QSORT_STACK_SIZE - 1, 1004 );\n\n      fpop ( lo, hi );\n      if (hi - lo < FALLBACK_QSORT_SMALL_THRESH) {\n         fallbackSimpleSort ( fmap, eclass, lo, hi );\n         continue;\n      }\n\n      /* Random partitioning.  Median of 3 sometimes fails to\n         avoid bad cases.  Median of 9 seems to help but \n         looks rather expensive.  This too seems to work but\n         is cheaper.  Guidance for the magic constants \n         7621 and 32768 is taken from Sedgewick's algorithms\n         book, chapter 35.\n      */\n      r = ((r * 7621) + 1) % 32768;\n      r3 = r % 3;\n      if (r3 == 0) med = eclass[fmap[lo]]; else\n      if (r3 == 1) med = eclass[fmap[(lo+hi)>>1]]; else\n                   med = eclass[fmap[hi]];\n\n      unLo = ltLo = lo;\n      unHi = gtHi = hi;\n\n      while (1) {\n         while (1) {\n            if (unLo > unHi) break;\n            n = (Int32)eclass[fmap[unLo]] - (Int32)med;\n            if (n == 0) { \n               fswap(fmap[unLo], fmap[ltLo]); \n               ltLo++; unLo++; \n               continue; \n            };\n            if (n > 0) break;\n            unLo++;\n         }\n         while (1) {\n            if (unLo > unHi) break;\n            n = (Int32)eclass[fmap[unHi]] - (Int32)med;\n            if (n == 0) { \n               fswap(fmap[unHi], fmap[gtHi]); \n               gtHi--; unHi--; \n               continue; \n            };\n            if (n < 0) break;\n            unHi--;\n         }\n         if (unLo > unHi) break;\n         fswap(fmap[unLo], fmap[unHi]); unLo++; unHi--;\n      }\n\n      AssertD ( unHi == unLo-1, \"fallbackQSort3(2)\" );\n\n      if (gtHi < ltLo) continue;\n\n      n = fmin(ltLo-lo, unLo-ltLo); fvswap(lo, unLo-n, n);\n      m = fmin(hi-gtHi, gtHi-unHi); fvswap(unLo, hi-m+1, m);\n\n      n = lo + unLo - ltLo - 1;\n      m = hi - (gtHi - unHi) + 1;\n\n      if (n - lo > hi - m) {\n         fpush ( lo, n );\n         fpush ( m, hi );\n      } else {\n         fpush ( m, hi );\n         fpush ( lo, n );\n      }\n   }\n}\n\n#undef fmin\n#undef fpush\n#undef fpop\n#undef fswap\n#undef fvswap\n#undef FALLBACK_QSORT_SMALL_THRESH\n#undef FALLBACK_QSORT_STACK_SIZE\n\n\n/*---------------------------------------------*/\n/* Pre:\n      nblock > 0\n      eclass exists for [0 .. nblock-1]\n      ((UChar*)eclass) [0 .. nblock-1] holds block\n      ptr exists for [0 .. nblock-1]\n\n   Post:\n      ((UChar*)eclass) [0 .. nblock-1] holds block\n      All other areas of eclass destroyed\n      fmap [0 .. nblock-1] holds sorted order\n      bhtab [ 0 .. 2+(nblock/32) ] destroyed\n*/\n\n#define       SET_BH(zz)  bhtab[(zz) >> 5] |= (1 << ((zz) & 31))\n#define     CLEAR_BH(zz)  bhtab[(zz) >> 5] &= ~(1 << ((zz) & 31))\n#define     ISSET_BH(zz)  (bhtab[(zz) >> 5] & (1 << ((zz) & 31)))\n#define      WORD_BH(zz)  bhtab[(zz) >> 5]\n#define UNALIGNED_BH(zz)  ((zz) & 0x01f)\n\nstatic\nvoid fallbackSort ( UInt32* fmap, \n                    UInt32* eclass, \n                    UInt32* bhtab,\n                    Int32   nblock,\n                    Int32   verb )\n{\n   Int32 ftab[257];\n   Int32 ftabCopy[256];\n   Int32 H, i, j, k, l, r, cc, cc1;\n   Int32 nNotDone;\n   Int32 nBhtab;\n   UChar* eclass8 = (UChar*)eclass;\n\n   /*--\n      Initial 1-char radix sort to generate\n      initial fmap and initial BH bits.\n   --*/\n   if (verb >= 4)\n      VPrintf0 ( \"        bucket sorting ...\\n\" );\n   for (i = 0; i < 257;    i++) ftab[i] = 0;\n   for (i = 0; i < nblock; i++) ftab[eclass8[i]]++;\n   for (i = 0; i < 256;    i++) ftabCopy[i] = ftab[i];\n   for (i = 1; i < 257;    i++) ftab[i] += ftab[i-1];\n\n   for (i = 0; i < nblock; i++) {\n      j = eclass8[i];\n      k = ftab[j] - 1;\n      ftab[j] = k;\n      fmap[k] = i;\n   }\n\n   nBhtab = 2 + (nblock / 32);\n   for (i = 0; i < nBhtab; i++) bhtab[i] = 0;\n   for (i = 0; i < 256; i++) SET_BH(ftab[i]);\n\n   /*--\n      Inductively refine the buckets.  Kind-of an\n      \"exponential radix sort\" (!), inspired by the\n      Manber-Myers suffix array construction algorithm.\n   --*/\n\n   /*-- set sentinel bits for block-end detection --*/\n   for (i = 0; i < 32; i++) { \n      SET_BH(nblock + 2*i);\n      CLEAR_BH(nblock + 2*i + 1);\n   }\n\n   /*-- the log(N) loop --*/\n   H = 1;\n   while (1) {\n\n      if (verb >= 4) \n         VPrintf1 ( \"        depth %6d has \", H );\n\n      j = 0;\n      for (i = 0; i < nblock; i++) {\n         if (ISSET_BH(i)) j = i;\n         k = fmap[i] - H; if (k < 0) k += nblock;\n         eclass[k] = j;\n      }\n\n      nNotDone = 0;\n      r = -1;\n      while (1) {\n\n   /*-- find the next non-singleton bucket --*/\n         k = r + 1;\n         while (ISSET_BH(k) && UNALIGNED_BH(k)) k++;\n         if (ISSET_BH(k)) {\n            while (WORD_BH(k) == 0xffffffff) k += 32;\n            while (ISSET_BH(k)) k++;\n         }\n         l = k - 1;\n         if (l >= nblock) break;\n         while (!ISSET_BH(k) && UNALIGNED_BH(k)) k++;\n         if (!ISSET_BH(k)) {\n            while (WORD_BH(k) == 0x00000000) k += 32;\n            while (!ISSET_BH(k)) k++;\n         }\n         r = k - 1;\n         if (r >= nblock) break;\n\n         /*-- now [l, r] bracket current bucket --*/\n         if (r > l) {\n            nNotDone += (r - l + 1);\n            fallbackQSort3 ( fmap, eclass, l, r );\n\n            /*-- scan bucket and generate header bits-- */\n            cc = -1;\n            for (i = l; i <= r; i++) {\n               cc1 = eclass[fmap[i]];\n               if (cc != cc1) { SET_BH(i); cc = cc1; };\n            }\n         }\n      }\n\n      if (verb >= 4) \n         VPrintf1 ( \"%6d unresolved strings\\n\", nNotDone );\n\n      H *= 2;\n      if (H > nblock || nNotDone == 0) break;\n   }\n\n   /*-- \n      Reconstruct the original block in\n      eclass8 [0 .. nblock-1], since the\n      previous phase destroyed it.\n   --*/\n   if (verb >= 4)\n      VPrintf0 ( \"        reconstructing block ...\\n\" );\n   j = 0;\n   for (i = 0; i < nblock; i++) {\n      while (ftabCopy[j] == 0) j++;\n      ftabCopy[j]--;\n      eclass8[fmap[i]] = (UChar)j;\n   }\n   AssertH ( j < 256, 1005 );\n}\n\n#undef       SET_BH\n#undef     CLEAR_BH\n#undef     ISSET_BH\n#undef      WORD_BH\n#undef UNALIGNED_BH\n\n\n/*---------------------------------------------*/\n/*--- The main, O(N^2 log(N)) sorting       ---*/\n/*--- algorithm.  Faster for \"normal\"       ---*/\n/*--- non-repetitive blocks.                ---*/\n/*---------------------------------------------*/\n\n/*---------------------------------------------*/\nstatic\n__inline__\nBool mainGtU ( UInt32  i1, \n               UInt32  i2,\n               UChar*  block, \n               UInt16* quadrant,\n               UInt32  nblock,\n               Int32*  budget )\n{\n   Int32  k;\n   UChar  c1, c2;\n   UInt16 s1, s2;\n\n   AssertD ( i1 != i2, \"mainGtU\" );\n   /* 1 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 2 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 3 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 4 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 5 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 6 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 7 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 8 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 9 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 10 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 11 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n   /* 12 */\n   c1 = block[i1]; c2 = block[i2];\n   if (c1 != c2) return (c1 > c2);\n   i1++; i2++;\n\n   k = nblock + 8;\n\n   do {\n      /* 1 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 2 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 3 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 4 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 5 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 6 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 7 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n      /* 8 */\n      c1 = block[i1]; c2 = block[i2];\n      if (c1 != c2) return (c1 > c2);\n      s1 = quadrant[i1]; s2 = quadrant[i2];\n      if (s1 != s2) return (s1 > s2);\n      i1++; i2++;\n\n      if (i1 >= nblock) i1 -= nblock;\n      if (i2 >= nblock) i2 -= nblock;\n\n      k -= 8;\n      (*budget)--;\n   }\n      while (k >= 0);\n\n   return False;\n}\n\n\n/*---------------------------------------------*/\n/*--\n   Knuth's increments seem to work better\n   than Incerpi-Sedgewick here.  Possibly\n   because the number of elems to sort is\n   usually small, typically <= 20.\n--*/\nstatic\nInt32 incs[14] = { 1, 4, 13, 40, 121, 364, 1093, 3280,\n                   9841, 29524, 88573, 265720,\n                   797161, 2391484 };\n\nstatic\nvoid mainSimpleSort ( UInt32* ptr,\n                      UChar*  block,\n                      UInt16* quadrant,\n                      Int32   nblock,\n                      Int32   lo, \n                      Int32   hi, \n                      Int32   d,\n                      Int32*  budget )\n{\n   Int32 i, j, h, bigN, hp;\n   UInt32 v;\n\n   bigN = hi - lo + 1;\n   if (bigN < 2) return;\n\n   hp = 0;\n   while (incs[hp] < bigN) hp++;\n   hp--;\n\n   for (; hp >= 0; hp--) {\n      h = incs[hp];\n\n      i = lo + h;\n      while (True) {\n\n         /*-- copy 1 --*/\n         if (i > hi) break;\n         v = ptr[i];\n         j = i;\n         while ( mainGtU ( \n                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget \n                 ) ) {\n            ptr[j] = ptr[j-h];\n            j = j - h;\n            if (j <= (lo + h - 1)) break;\n         }\n         ptr[j] = v;\n         i++;\n\n         /*-- copy 2 --*/\n         if (i > hi) break;\n         v = ptr[i];\n         j = i;\n         while ( mainGtU ( \n                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget \n                 ) ) {\n            ptr[j] = ptr[j-h];\n            j = j - h;\n            if (j <= (lo + h - 1)) break;\n         }\n         ptr[j] = v;\n         i++;\n\n         /*-- copy 3 --*/\n         if (i > hi) break;\n         v = ptr[i];\n         j = i;\n         while ( mainGtU ( \n                    ptr[j-h]+d, v+d, block, quadrant, nblock, budget \n                 ) ) {\n            ptr[j] = ptr[j-h];\n            j = j - h;\n            if (j <= (lo + h - 1)) break;\n         }\n         ptr[j] = v;\n         i++;\n\n         if (*budget < 0) return;\n      }\n   }\n}\n\n\n/*---------------------------------------------*/\n/*--\n   The following is an implementation of\n   an elegant 3-way quicksort for strings,\n   described in a paper \"Fast Algorithms for\n   Sorting and Searching Strings\", by Robert\n   Sedgewick and Jon L. Bentley.\n--*/\n\n#define mswap(zz1, zz2) \\\n   { Int32 zztmp = zz1; zz1 = zz2; zz2 = zztmp; }\n\n#define mvswap(zzp1, zzp2, zzn)       \\\n{                                     \\\n   Int32 yyp1 = (zzp1);               \\\n   Int32 yyp2 = (zzp2);               \\\n   Int32 yyn  = (zzn);                \\\n   while (yyn > 0) {                  \\\n      mswap(ptr[yyp1], ptr[yyp2]);    \\\n      yyp1++; yyp2++; yyn--;          \\\n   }                                  \\\n}\n\nstatic \n__inline__\nUChar mmed3 ( UChar a, UChar b, UChar c )\n{\n   UChar t;\n   if (a > b) { t = a; a = b; b = t; };\n   if (b > c) { \n      b = c;\n      if (a > b) b = a;\n   }\n   return b;\n}\n\n#define mmin(a,b) ((a) < (b)) ? (a) : (b)\n\n#define mpush(lz,hz,dz) { stackLo[sp] = lz; \\\n                          stackHi[sp] = hz; \\\n                          stackD [sp] = dz; \\\n                          sp++; }\n\n#define mpop(lz,hz,dz) { sp--;             \\\n                         lz = stackLo[sp]; \\\n                         hz = stackHi[sp]; \\\n                         dz = stackD [sp]; }\n\n\n#define mnextsize(az) (nextHi[az]-nextLo[az])\n\n#define mnextswap(az,bz)                                        \\\n   { Int32 tz;                                                  \\\n     tz = nextLo[az]; nextLo[az] = nextLo[bz]; nextLo[bz] = tz; \\\n     tz = nextHi[az]; nextHi[az] = nextHi[bz]; nextHi[bz] = tz; \\\n     tz = nextD [az]; nextD [az] = nextD [bz]; nextD [bz] = tz; }\n\n\n#define MAIN_QSORT_SMALL_THRESH 20\n#define MAIN_QSORT_DEPTH_THRESH (BZ_N_RADIX + BZ_N_QSORT)\n#define MAIN_QSORT_STACK_SIZE 100\n\nstatic\nvoid mainQSort3 ( UInt32* ptr,\n                  UChar*  block,\n                  UInt16* quadrant,\n                  Int32   nblock,\n                  Int32   loSt, \n                  Int32   hiSt, \n                  Int32   dSt,\n                  Int32*  budget )\n{\n   Int32 unLo, unHi, ltLo, gtHi, n, m, med;\n   Int32 sp, lo, hi, d;\n\n   Int32 stackLo[MAIN_QSORT_STACK_SIZE];\n   Int32 stackHi[MAIN_QSORT_STACK_SIZE];\n   Int32 stackD [MAIN_QSORT_STACK_SIZE];\n\n   Int32 nextLo[3];\n   Int32 nextHi[3];\n   Int32 nextD [3];\n\n   sp = 0;\n   mpush ( loSt, hiSt, dSt );\n\n   while (sp > 0) {\n\n      AssertH ( sp < MAIN_QSORT_STACK_SIZE - 2, 1001 );\n\n      mpop ( lo, hi, d );\n      if (hi - lo < MAIN_QSORT_SMALL_THRESH || \n          d > MAIN_QSORT_DEPTH_THRESH) {\n         mainSimpleSort ( ptr, block, quadrant, nblock, lo, hi, d, budget );\n         if (*budget < 0) return;\n         continue;\n      }\n\n      med = (Int32) \n            mmed3 ( block[ptr[ lo         ]+d],\n                    block[ptr[ hi         ]+d],\n                    block[ptr[ (lo+hi)>>1 ]+d] );\n\n      unLo = ltLo = lo;\n      unHi = gtHi = hi;\n\n      while (True) {\n         while (True) {\n            if (unLo > unHi) break;\n            n = ((Int32)block[ptr[unLo]+d]) - med;\n            if (n == 0) { \n               mswap(ptr[unLo], ptr[ltLo]); \n               ltLo++; unLo++; continue; \n            };\n            if (n >  0) break;\n            unLo++;\n         }\n         while (True) {\n            if (unLo > unHi) break;\n            n = ((Int32)block[ptr[unHi]+d]) - med;\n            if (n == 0) { \n               mswap(ptr[unHi], ptr[gtHi]); \n               gtHi--; unHi--; continue; \n            };\n            if (n <  0) break;\n            unHi--;\n         }\n         if (unLo > unHi) break;\n         mswap(ptr[unLo], ptr[unHi]); unLo++; unHi--;\n      }\n\n      AssertD ( unHi == unLo-1, \"mainQSort3(2)\" );\n\n      if (gtHi < ltLo) {\n         mpush(lo, hi, d+1 );\n         continue;\n      }\n\n      n = mmin(ltLo-lo, unLo-ltLo); mvswap(lo, unLo-n, n);\n      m = mmin(hi-gtHi, gtHi-unHi); mvswap(unLo, hi-m+1, m);\n\n      n = lo + unLo - ltLo - 1;\n      m = hi - (gtHi - unHi) + 1;\n\n      nextLo[0] = lo;  nextHi[0] = n;   nextD[0] = d;\n      nextLo[1] = m;   nextHi[1] = hi;  nextD[1] = d;\n      nextLo[2] = n+1; nextHi[2] = m-1; nextD[2] = d+1;\n\n      if (mnextsize(0) < mnextsize(1)) mnextswap(0,1);\n      if (mnextsize(1) < mnextsize(2)) mnextswap(1,2);\n      if (mnextsize(0) < mnextsize(1)) mnextswap(0,1);\n\n      AssertD (mnextsize(0) >= mnextsize(1), \"mainQSort3(8)\" );\n      AssertD (mnextsize(1) >= mnextsize(2), \"mainQSort3(9)\" );\n\n      mpush (nextLo[0], nextHi[0], nextD[0]);\n      mpush (nextLo[1], nextHi[1], nextD[1]);\n      mpush (nextLo[2], nextHi[2], nextD[2]);\n   }\n}\n\n#undef mswap\n#undef mvswap\n#undef mpush\n#undef mpop\n#undef mmin\n#undef mnextsize\n#undef mnextswap\n#undef MAIN_QSORT_SMALL_THRESH\n#undef MAIN_QSORT_DEPTH_THRESH\n#undef MAIN_QSORT_STACK_SIZE\n\n\n/*---------------------------------------------*/\n/* Pre:\n      nblock > N_OVERSHOOT\n      block32 exists for [0 .. nblock-1 +N_OVERSHOOT]\n      ((UChar*)block32) [0 .. nblock-1] holds block\n      ptr exists for [0 .. nblock-1]\n\n   Post:\n      ((UChar*)block32) [0 .. nblock-1] holds block\n      All other areas of block32 destroyed\n      ftab [0 .. 65536 ] destroyed\n      ptr [0 .. nblock-1] holds sorted order\n      if (*budget < 0), sorting was abandoned\n*/\n\n#define BIGFREQ(b) (ftab[((b)+1) << 8] - ftab[(b) << 8])\n#define SETMASK (1 << 21)\n#define CLEARMASK (~(SETMASK))\n\nstatic\nvoid mainSort ( UInt32* ptr, \n                UChar*  block,\n                UInt16* quadrant, \n                UInt32* ftab,\n                Int32   nblock,\n                Int32   verb,\n                Int32*  budget )\n{\n   Int32  i, j, k, ss, sb;\n   Int32  runningOrder[256];\n   Bool   bigDone[256];\n   Int32  copyStart[256];\n   Int32  copyEnd  [256];\n   UChar  c1;\n   Int32  numQSorted;\n   UInt16 s;\n   if (verb >= 4) VPrintf0 ( \"        main sort initialise ...\\n\" );\n\n   /*-- set up the 2-byte frequency table --*/\n   for (i = 65536; i >= 0; i--) ftab[i] = 0;\n\n   j = block[0] << 8;\n   i = nblock-1;\n   for (; i >= 3; i -= 4) {\n      quadrant[i] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i]) << 8);\n      ftab[j]++;\n      quadrant[i-1] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i-1]) << 8);\n      ftab[j]++;\n      quadrant[i-2] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i-2]) << 8);\n      ftab[j]++;\n      quadrant[i-3] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i-3]) << 8);\n      ftab[j]++;\n   }\n   for (; i >= 0; i--) {\n      quadrant[i] = 0;\n      j = (j >> 8) | ( ((UInt16)block[i]) << 8);\n      ftab[j]++;\n   }\n\n   /*-- (emphasises close relationship of block & quadrant) --*/\n   for (i = 0; i < BZ_N_OVERSHOOT; i++) {\n      block   [nblock+i] = block[i];\n      quadrant[nblock+i] = 0;\n   }\n\n   if (verb >= 4) VPrintf0 ( \"        bucket sorting ...\\n\" );\n\n   /*-- Complete the initial radix sort --*/\n   for (i = 1; i <= 65536; i++) ftab[i] += ftab[i-1];\n\n   s = block[0] << 8;\n   i = nblock-1;\n   for (; i >= 3; i -= 4) {\n      s = (s >> 8) | (block[i] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i;\n      s = (s >> 8) | (block[i-1] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i-1;\n      s = (s >> 8) | (block[i-2] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i-2;\n      s = (s >> 8) | (block[i-3] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i-3;\n   }\n   for (; i >= 0; i--) {\n      s = (s >> 8) | (block[i] << 8);\n      j = ftab[s] -1;\n      ftab[s] = j;\n      ptr[j] = i;\n   }\n\n   /*--\n      Now ftab contains the first loc of every small bucket.\n      Calculate the running order, from smallest to largest\n      big bucket.\n   --*/\n   for (i = 0; i <= 255; i++) {\n      bigDone     [i] = False;\n      runningOrder[i] = i;\n   }\n\n   {\n      Int32 vv;\n      Int32 h = 1;\n      do h = 3 * h + 1; while (h <= 256);\n      do {\n         h = h / 3;\n         for (i = h; i <= 255; i++) {\n            vv = runningOrder[i];\n            j = i;\n            while ( BIGFREQ(runningOrder[j-h]) > BIGFREQ(vv) ) {\n               runningOrder[j] = runningOrder[j-h];\n               j = j - h;\n               if (j <= (h - 1)) goto zero;\n            }\n            zero:\n            runningOrder[j] = vv;\n         }\n      } while (h != 1);\n   }\n\n   /*--\n      The main sorting loop.\n   --*/\n\n   numQSorted = 0;\n\n   for (i = 0; i <= 255; i++) {\n\n      /*--\n         Process big buckets, starting with the least full.\n         Basically this is a 3-step process in which we call\n         mainQSort3 to sort the small buckets [ss, j], but\n         also make a big effort to avoid the calls if we can.\n      --*/\n      ss = runningOrder[i];\n\n      /*--\n         Step 1:\n         Complete the big bucket [ss] by quicksorting\n         any unsorted small buckets [ss, j], for j != ss.  \n         Hopefully previous pointer-scanning phases have already\n         completed many of the small buckets [ss, j], so\n         we don't have to sort them at all.\n      --*/\n      for (j = 0; j <= 255; j++) {\n         if (j != ss) {\n            sb = (ss << 8) + j;\n            if ( ! (ftab[sb] & SETMASK) ) {\n               Int32 lo = ftab[sb]   & CLEARMASK;\n               Int32 hi = (ftab[sb+1] & CLEARMASK) - 1;\n               if (hi > lo) {\n                  if (verb >= 4)\n                     VPrintf4 ( \"        qsort [0x%x, 0x%x]   \"\n                                \"done %d   this %d\\n\",\n                                ss, j, numQSorted, hi - lo + 1 );\n                  mainQSort3 ( \n                     ptr, block, quadrant, nblock, \n                     lo, hi, BZ_N_RADIX, budget \n                  );   \n                  numQSorted += (hi - lo + 1);\n                  if (*budget < 0) return;\n               }\n            }\n            ftab[sb] |= SETMASK;\n         }\n      }\n\n      AssertH ( !bigDone[ss], 1006 );\n\n      /*--\n         Step 2:\n         Now scan this big bucket [ss] so as to synthesise the\n         sorted order for small buckets [t, ss] for all t,\n         including, magically, the bucket [ss,ss] too.\n         This will avoid doing Real Work in subsequent Step 1's.\n      --*/\n      {\n         for (j = 0; j <= 255; j++) {\n            copyStart[j] =  ftab[(j << 8) + ss]     & CLEARMASK;\n            copyEnd  [j] = (ftab[(j << 8) + ss + 1] & CLEARMASK) - 1;\n         }\n         for (j = ftab[ss << 8] & CLEARMASK; j < copyStart[ss]; j++) {\n            k = ptr[j]-1; if (k < 0) k += nblock;\n            c1 = block[k];\n            if (!bigDone[c1])\n               ptr[ copyStart[c1]++ ] = k;\n         }\n         for (j = (ftab[(ss+1) << 8] & CLEARMASK) - 1; j > copyEnd[ss]; j--) {\n            k = ptr[j]-1; if (k < 0) k += nblock;\n            c1 = block[k];\n            if (!bigDone[c1]) \n               ptr[ copyEnd[c1]-- ] = k;\n         }\n      }\n\n      AssertH ( (copyStart[ss]-1 == copyEnd[ss])\n                || \n                /* Extremely rare case missing in bzip2-1.0.0 and 1.0.1.\n                   Necessity for this case is demonstrated by compressing \n                   a sequence of approximately 48.5 million of character \n                   251; 1.0.0/1.0.1 will then die here. */\n                (copyStart[ss] == 0 && copyEnd[ss] == nblock-1),\n                1007 )\n\n      for (j = 0; j <= 255; j++) ftab[(j << 8) + ss] |= SETMASK;\n\n      /*--\n         Step 3:\n         The [ss] big bucket is now done.  Record this fact,\n         and update the quadrant descriptors.  Remember to\n         update quadrants in the overshoot area too, if\n         necessary.  The \"if (i < 255)\" test merely skips\n         this updating for the last bucket processed, since\n         updating for the last bucket is pointless.\n\n         The quadrant array provides a way to incrementally\n         cache sort orderings, as they appear, so as to \n         make subsequent comparisons in fullGtU() complete\n         faster.  For repetitive blocks this makes a big\n         difference (but not big enough to be able to avoid\n         the fallback sorting mechanism, exponential radix sort).\n\n         The precise meaning is: at all times:\n\n            for 0 <= i < nblock and 0 <= j <= nblock\n\n            if block[i] != block[j], \n\n               then the relative values of quadrant[i] and \n                    quadrant[j] are meaningless.\n\n               else {\n                  if quadrant[i] < quadrant[j]\n                     then the string starting at i lexicographically\n                     precedes the string starting at j\n\n                  else if quadrant[i] > quadrant[j]\n                     then the string starting at j lexicographically\n                     precedes the string starting at i\n\n                  else\n                     the relative ordering of the strings starting\n                     at i and j has not yet been determined.\n               }\n      --*/\n      bigDone[ss] = True;\n\n      if (i < 255) {\n         Int32 bbStart  = ftab[ss << 8] & CLEARMASK;\n         Int32 bbSize   = (ftab[(ss+1) << 8] & CLEARMASK) - bbStart;\n         Int32 shifts   = 0;\n\n         while ((bbSize >> shifts) > 65534) shifts++;\n\n         for (j = bbSize-1; j >= 0; j--) {\n            Int32 a2update     = ptr[bbStart + j];\n            UInt16 qVal        = (UInt16)(j >> shifts);\n            quadrant[a2update] = qVal;\n            if (a2update < BZ_N_OVERSHOOT)\n               quadrant[a2update + nblock] = qVal;\n         }\n         AssertH ( ((bbSize-1) >> shifts) <= 65535, 1002 );\n      }\n\n   }\n\n   if (verb >= 4)\n      VPrintf3 ( \"        %d pointers, %d sorted, %d scanned\\n\",\n                 nblock, numQSorted, nblock - numQSorted );\n}\n\n#undef BIGFREQ\n#undef SETMASK\n#undef CLEARMASK\n\n\n/*---------------------------------------------*/\n/* Pre:\n      nblock > 0\n      arr2 exists for [0 .. nblock-1 +N_OVERSHOOT]\n      ((UChar*)arr2)  [0 .. nblock-1] holds block\n      arr1 exists for [0 .. nblock-1]\n\n   Post:\n      ((UChar*)arr2) [0 .. nblock-1] holds block\n      All other areas of block destroyed\n      ftab [ 0 .. 65536 ] destroyed\n      arr1 [0 .. nblock-1] holds sorted order\n*/\nvoid BZ2_blockSort ( EState* s )\n{\n   UInt32* ptr    = s->ptr; \n   UChar*  block  = s->block;\n   UInt32* ftab   = s->ftab;\n   Int32   nblock = s->nblock;\n   Int32   verb   = s->verbosity;\n   Int32   wfact  = s->workFactor;\n   UInt16* quadrant;\n   Int32   budget;\n   Int32   budgetInit;\n   Int32   i;\n\n   if (nblock < 10000) {\n      fallbackSort ( s->arr1, s->arr2, ftab, nblock, verb );\n   } else {\n      /* Calculate the location for quadrant, remembering to get\n         the alignment right.  Assumes that &(block[0]) is at least\n         2-byte aligned -- this should be ok since block is really\n         the first section of arr2.\n      */\n      i = nblock+BZ_N_OVERSHOOT;\n      if (i & 1) i++;\n      quadrant = (UInt16*)(&(block[i]));\n\n      /* (wfact-1) / 3 puts the default-factor-30\n         transition point at very roughly the same place as \n         with v0.1 and v0.9.0.  \n         Not that it particularly matters any more, since the\n         resulting compressed stream is now the same regardless\n         of whether or not we use the main sort or fallback sort.\n      */\n      if (wfact < 1  ) wfact = 1;\n      if (wfact > 100) wfact = 100;\n      budgetInit = nblock * ((wfact-1) / 3);\n      budget = budgetInit;\n\n      mainSort ( ptr, block, quadrant, ftab, nblock, verb, &budget );\n      if (verb >= 3) \n         VPrintf3 ( \"      %d work, %d block, ratio %5.2f\\n\",\n                    budgetInit - budget,\n                    nblock, \n                    (float)(budgetInit - budget) /\n                    (float)(nblock==0 ? 1 : nblock) ); \n      if (budget < 0) {\n         if (verb >= 2) \n            VPrintf0 ( \"    too repetitive; using fallback\"\n                       \" sorting algorithm\\n\" );\n         fallbackSort ( s->arr1, s->arr2, ftab, nblock, verb );\n      }\n   }\n\n   s->origPtr = -1;\n   for (i = 0; i < s->nblock; i++)\n      if (ptr[i] == 0)\n         { s->origPtr = i; break; };\n\n   AssertH( s->origPtr != -1, 1003 );\n}\n\n\n/*-------------------------------------------------------------*/\n/*--- end                                       blocksort.c ---*/\n/*-------------------------------------------------------------*/\n"
    )
  endif()
  include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/bzip2.cmake")

  set(LIBBZ2_SOURCES 3rdparty/bzip2/blocksort.c 3rdparty/bzip2/bzlib.c 3rdparty/bzip2/compress.c 3rdparty/bzip2/crctable.c 3rdparty/bzip2/decompress.c 3rdparty/bzip2/huffman.c 3rdparty/bzip2/randtable.c)
  add_library(bz2 ${LIBBZ2_SOURCES})
  set_target_properties(bz2 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bzip2" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bzip2")
  link_directories(BEFORE "${CMAKE_BINARY_DIR}/bzip2")

  set(BZIP2_LIBRARY_DEBUG bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY_RELEASE bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY bz2 CACHE FILEPATH "" FORCE)
  set(BZIP2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rdparty/bzip2 CACHE PATH "" FORCE)
endif(BUILD_BZIP2)

function(DEFINE_HAVE OUTVAR)
  set(OUT "")
  foreach(DEF ${ARGN})
    # message("Check definition ${DEF}: ${${DEF}}")
    if(${DEF} STREQUAL 1)
      list(APPEND OUT "${DEF}=1")
    endif(${DEF} STREQUAL 1)
  endforeach(DEF ${ARGN})
  set("${OUTVAR}" "${OUT}" PARENT_SCOPE)

endfunction(DEFINE_HAVE)

if(BUILD_LZMA)
  message("Building xz from 3rdparty/xz")
  set(LZMA_VERSION "5.2.5")
  set(LZMA_DISABLE_TESTS TRUE)
  set(LZMA_STATIC TRUE)
  set(LZMA_NO_INSTALL TRUE)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/xz/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/xz-${LZMA_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/xz")
      file(DOWNLOAD "https://distfiles.macports.org/xz/xz-${LZMA_VERSION}.tar.bz2" "${CMAKE_SOURCE_DIR}/3rdparty/xz-${LZMA_VERSION}.tar.bz2" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xjf "${CMAKE_SOURCE_DIR}/3rdparty/xz-${LZMA_VERSION}.tar.bz2")
    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/xz-${LZMA_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/xz")

  endif()
  include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz.cmake")

  # configure_file("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/xz-config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/xz/config.h")

  # configure_file("${CMAKE_SOURCE_DIR}/3rdparty/lzma-config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/3rdparty/xz/config.h")

  file(
    GLOB
    LIBLZMA_SOURCES
    # 3rdparty/xz/debug/*.c 3rdparty/xz/lib/*.c 3rdparty/xz/src/common/*.c
    3rdparty/xz/src/liblzma/check/check.c
    3rdparty/xz/src/liblzma/check/*_fast.c
    3rdparty/xz/src/liblzma/check/*_table.c
    3rdparty/xz/src/liblzma/check/sha256.c
    # 3rdparty/xz/src/liblzma/check/*.S
    3rdparty/xz/src/liblzma/common/*block_util.c
    3rdparty/xz/src/liblzma/common/*coder.c
    3rdparty/xz/src/liblzma/common/*coder_memusage.c
    # 3rdparty/xz/src/liblzma/common/*coder_mt.c
    3rdparty/xz/src/liblzma/common/*common.c
    3rdparty/xz/src/liblzma/common/*easy_preset.c
    3rdparty/xz/src/liblzma/common/*filter_common.c
    3rdparty/xz/src/liblzma/common/*hardware_cputhreads.c
    3rdparty/xz/src/liblzma/common/*hardware_physmem.c
    3rdparty/xz/src/liblzma/common/*index.c
    3rdparty/xz/src/liblzma/common/*index_hash.c
    3rdparty/xz/src/liblzma/common/*outqueue.c
    3rdparty/xz/src/liblzma/common/*stream_flags_common.c
    3rdparty/xz/src/liblzma/common/*vli_size.c
    3rdparty/xz/src/liblzma/delta/*.c
    3rdparty/xz/src/liblzma/lz/*.c
    3rdparty/xz/src/liblzma/lzma/*.c
    3rdparty/xz/src/liblzma/rangecoder/*.c
    3rdparty/xz/src/liblzma/simple/*.c)
  # list(FILTER LIBLZMA_SOURCES EXCLUDE REGEX ".*_mt\\..*")

  if(LIBLZMA_SOURCES)
    add_library(lzma ${LIBLZMA_SOURCES})
    set_target_properties(lzma PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xz" LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xz")
    set(LZMA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/xz")
    set(LIBLZMA_INCLUDE_DIRS ${LZMA_SOURCE_DIR}/src/common ${LZMA_SOURCE_DIR}/src/liblzma/lzma ${LZMA_SOURCE_DIR}/src/liblzma/lz ${LZMA_SOURCE_DIR}/src/liblzma/check ${LZMA_SOURCE_DIR}/src/liblzma/simple ${LZMA_SOURCE_DIR}/src/liblzma/delta ${LZMA_SOURCE_DIR}/src/liblzma/rangecoder ${LZMA_SOURCE_DIR}/src/liblzma/common ${LZMA_SOURCE_DIR}/src/liblzma/api ${CMAKE_BINARY_DIR}/3rdparty/xz)
    define_have(LIBLZMA_DEFS HAVE_STDINT_H HAVE_STDLIB_H HAVE_STDBOOL_H HAVE_INTTYPES_H HAVE_LIMITS_H HAVE_STRING_H HAVE_STRINGS_H HAVE_MEMORY_H HAVE_SYS_PARAM_H HAVE_SYS_TIME_H HAVE_SYS_TYPES_H HAVE_LZMA_H)
    # message("LIBLZMA_DEFS: ${LIBLZMA_DEFS}")
    set_target_properties(lzma PROPERTIES COMPILE_DEFINITIONS "${LIBLZMA_DEFS}" INCLUDE_DIRECTORIES "${LIBLZMA_INCLUDE_DIRS}")
  endif(LIBLZMA_SOURCES)

  link_directories(BEFORE "${CMAKE_BINARY_DIR}/xz")
  set(LIBLZMA_LIBRARY lzma CACHE FILEPATH "" FORCE)
  set(LIBLZMA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/xz/src/liblzma/api" CACHE FILEPATH "" FORCE)
endif(BUILD_LZMA)

if(BUILD_SSL)
  if(MUSL)
    add_definitions(-DOPENSSL_NO_SECURE_MEMORY=1)
  endif(MUSL)
  if(DIET OR MUSL)
    set(CHK_DLFCN FALSE)
    set(DSO_NONE ON)
  endif(DIET OR MUSL)
  set(WITH_APPS FALSE)
  set(OPENSSL_NO_INSTALL TRUE)
  set(OPENSSL_VERSION "1.1.1h")
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/openssl-${OPENSSL_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
      file(DOWNLOAD "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz" "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}.tar.gz")

    file(
      WRITE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1h-getrandom.patch"
      "--- 3rdparty/openssl-1.1.1h/crypto/rand/rand_unix.c\t2020-09-22 14:55:07.000000000 +0200\n+++ 3rdparty/openssl/crypto/rand/rand_unix.c\t2020-11-26 06:54:18.694873358 +0100\n@@ -19,6 +19,9 @@\n #include \"crypto/rand.h\"\n #include <stdio.h>\n #include \"internal/dso.h\"\n+#ifdef __dietlibc__\n+# include <sys/random.h>\n+#endif\n #ifdef __linux\n # include <sys/syscall.h>\n # ifdef DEVRANDOM_WAIT\n@@ -389,7 +392,9 @@\n #  endif\n \n     /* Linux supports this since version 3.17 */\n-#  if defined(__linux) && defined(__NR_getrandom)\n+#  if defined(__dietlibc__)\n+    return getrandom(buf, buflen, 0);\n+#  elif defined(__linux) && defined(__NR_getrandom)\n     return syscall(__NR_getrandom, buf, buflen, 0);\n #  elif (defined(__FreeBSD__) || defined(__NetBSD__)) && defined(KERN_ARND)\n     return sysctl_random(buf, buflen);\n"
    )
    execute_process(COMMAND patch -p0 INPUT_FILE "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1h-getrandom.patch" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
    # exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty/openssl" -xvzf "${CMAKE_SOURCE_DIR}/3rdparty/openssl-cmake.tar.gz")

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/openssl-${OPENSSL_VERSION}.tar.gz")

    include("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl.cmake")

  endif()

  add_definitions(-DOPENSSL_NO_UI_CONSOLE=1)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_NO_UI_CONSOLE=1")

  message(STATUS "Configuring OpenSSL...")
  add_subdirectory(3rdparty/openssl)

  set(OPENSSL_LIBRARIES ssl crypto CACHE STRING "" FORCE)
  set(OPENSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/openssl/include" CACHE FILEPATH "" FORCE)

  # set(OPENSSL_SSL_LIBRARY "${CMAKE_BINARY_DIR}/3rdparty/openssl/ssl/libssl.a" CACHE  FILEPATH "SSL library") set(OPENSSL_CRYPTO_LIBRARY "${CMAKE_BINARY_DIR}/3rdparty/openssl/crypto/libcrypto.a" CACHE  FILEPATH "Cryptography library")
  set(OPENSSL_SSL_LIBRARY libssl CACHE STRING "SSL library" FORCE)
  set(OPENSSL_CRYPTO_LIBRARY libcrypto CACHE FILEPATH "Cryptography library" FORCE)

endif()

if(USE_SSL)
  add_definitions(-DHAVE_OPENSSL=1 -DOPENSSL_API_COMPAT=0x10200000L)
  if(OPENSSL_INCLUDE_DIR)
    include_directories(${CMAKE_BINARY_DIR}/3rdparty/openssl/include ${OPENSSL_INCLUDE_DIR})
  endif()

  # if(NOT OPENSSL_LIBRARIES) if(OPENSSL_CRYPTO_LIBRARY OR OPENSSL_SSL_LIBRARY)
  set(OPENSSL_LIBARRIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
  # endif() endif()
endif(USE_SSL)

if(USE_BROTLI AND NOT BROTLI_FOUND)
  set(BUILD_BROTLI ON)
endif(USE_BROTLI AND NOT BROTLI_FOUND)

if(BUILD_BROTLI)
  message("Building brotli from 3rdparty/brotli")
  set(BROTLI_VERSION "1.0.9")
  set(BROTLI_DISABLE_TESTS TRUE)
  set(BROTLI_STATIC TRUE)
  set(BROTLI_NO_INSTALL TRUE)

  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/brotli/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/brotli-${BROTLI_VERSION}.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/brotli")
      file(DOWNLOAD "https://github.com/google/brotli/archive/v${BROTLI_VERSION}.tar.gz" "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}.tar.gz")
    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/brotli-${BROTLI_VERSION}" "${CMAKE_SOURCE_DIR}/3rdparty/brotli")

  endif() # file(READ "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/brotli.cmake.txt" BROTLI_CMAKELISTS_TXT) file(WRITE  "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/brotli/CMakeLists.txt"  "${BROTLI_CMAKELISTS_TXT}")
  set(BROTLI_STATIC TRUE)
  set(BROTLI_NO_INSTALL TRUE)

  add_subdirectory(3rdparty/brotli)
endif(BUILD_BROTLI)

link_directories(AFTER "${CMAKE_BINARY_DIR}")

file(TO_NATIVE_PATH "/" PATH_SEPARATOR)
message("The systems's path separator is '${PATH_SEPARATOR}'")

# message("BZIP2_INCLUDE_DIR: ${BZIP2_INCLUDE_DIR}") message("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}") message("LIBLZMA_INCLUDE_DIR: ${LIBLZMA_INCLUDE_DIR}") message("LIBLZMA_INCLUDE_DIRS: ${LIBLZMA_INCLUDE_DIRS}") message("OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}") message("ZLIB_INCLUDE_DIR: ${ZLIB_INCLUDE_DIR}")

set(iarray_SOURCES lib/cas.S)

set(MODSUBDIRS alloc array binfmt buffer byte case cb cbmap charbuf coff dir dns elf env errmsg fmt gpio hashmap hmap http iarray ihex io json list map mmap ndelay omf open path pe playlist process range rdir safemult scan seek set sig slist socket  str stralloc strarray strlist tai taia textbuf textcode tls uint16 uint32 uint64 unix wait xml ucs utf8)

foreach(LIBNAME ${MODSUBDIRS})
  file(GLOB MODHEADERS lib/${LIBNAME}.h lib/${LIBNAME}_*.h)
  file(GLOB MODSOURCES lib/${LIBNAME}/*.c)
  if(NOT MODSOURCES)
    file(GLOB MODSOURCES lib/${LIBNAME}_*.c)
  endif()
  set(EXTRA_SOURCES ${${LIBNAME}_SOURCES})
  if(EXTRA_SOURCES)
    message(STATUS "${LIBNAME} extra sources: ${EXTRA_SOURCES}")
  endif(EXTRA_SOURCES)
  add_library("${LIBNAME}" STATIC ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
  if(BUILD_SHARED_LIBS)
    add_library("${LIBNAME}-shared" SHARED ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
    set_target_properties("${LIBNAME}-shared" PROPERTIES PREFIX "lib" COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS} -fPIC")
  endif(BUILD_SHARED_LIBS)
  set_target_properties("${LIBNAME}" PROPERTIES PREFIX "" COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS} ${${LIBNAME}_COMPILE_FLAGS}")

endforeach(LIBNAME)

set_target_properties(io PROPERTIES COMPILE_FLAGS "${MUX_DEFS}")

foreach(NETLIB dns http io ndelay socket)
  target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()

if(BUILD_SSL)
  set(LIBSSL libssl libcrypto)
else(BUILD_SSL)
  if(LINK_STATIC)
    set(LIBSSL ${OPENSSL_STATIC_LIBRARIES})
  else(LINK_STATIC)
    set(LIBSSL ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
  endif(LINK_STATIC)
endif(BUILD_SSL)

dump(CMAKE_EXE_LINKER_FLAGS LIBSSL)

target_link_libraries(unix stralloc buffer ndelay)
target_link_libraries(array safemult byte str)
target_link_libraries(binfmt mmap byte)
target_link_libraries(buffer alloc byte fmt mmap open str ${ZLIB_LIBRARY})
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(dns byte case fmt io open socket str taia uint16 ${LIBIPHLPAPI})
target_link_libraries(dir stralloc)
target_link_libraries(errmsg str)
target_link_libraries(elf byte str range mmap uint64 uint32 uint16)
target_link_libraries(fmt byte str uint64 uint32)
target_link_libraries(gpio mmap)
target_link_libraries(hmap str)
target_link_libraries(http dns errmsg byte io scan socket stralloc str ${LIBSSL})
target_link_libraries(io buffer iarray array byte fmt open taia ${LIBMSWSOCK})
target_link_libraries(json charbuf hmap scan)
target_link_libraries(map str)
target_link_libraries(mmap seek open scan ${LIBPSAPI})
target_link_libraries(open str uint32)
target_link_libraries(path dir strlist)
target_link_libraries(pe uint64 uint32 uint16)
target_link_libraries(playlist byte fmt scan str xml)
target_link_libraries(rdir dir)
target_link_libraries(scan byte str)
target_link_libraries(socket byte fmt ndelay scan uint16 ${LIBMSWSOCK})
target_link_libraries(str byte)
target_link_libraries(stralloc alloc open byte fmt str)
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
target_link_libraries(tls iarray)

set(genmakefile_SOURCES mplab.c ini.c ini.h genmakefile.h debug.c debug.h)

if(OPENSSL_LIB_DIR)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${OPENSSL_LIB_DIR}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  option(USE_EFENCE "Enable electric fence" OFF)
endif()

if(USE_EFENCE)
  if(DIET)
    set(ELECTRICFENCE_LIBRARY efence pthread)
  else(DIET)
    set(ELECTRICFENCE_LIBRARY efence)
  endif(DIET)
endif(USE_EFENCE)

add_definitions(-DHAVE_CONFIG_H=1)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

set(COMMON_LIBS strlist array safemult scan str byte mmap open fmt)
file(GLOB MODHEADERS lib/*.h)

function(add_program PROG)
  unset(INCLUDES)
  unset(LIBS)
  foreach(DEP ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
      list(APPEND "${PROG}_SOURCES" "${DEP}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
      list(FIND MODSUBDIRS "${DEP}" I)
      if(I GREATER -1)
        list(APPEND INCLUDES "lib/${DEP}.h")
      endif(I GREATER -1)
      list(APPEND DEPS ${DEP})
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
  endforeach(DEP ${ARGN})
  # message("Program '${PROG}' dependencies: ${INCLUDES}")
  add_executable(${PROG} ${PROG}.c ${${PROG}_SOURCES} ${INCLUDES})
  target_link_libraries(${PROG} ${DEPS} ${ELECTRICFENCE_LIBRARY} ${EXTRA_LIBRARIES})
  install(TARGETS ${PROG} DESTINATION bin)
  if(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
    set_target_properties(${PROG} PROPERTIES LINK_FLAGS "-L${OPENSSL_LIB_DIR} ${EXE_LINKER_FLAGS}" COMPILE_FLAGS "")
  endif(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
  if(EMSCRIPTEN)
    set_target_properties(${PROG} PROPERTIES SUFFIX ".${EMSCRIPTEN_EXE_SUFFIX}")
  endif(EMSCRIPTEN)
endfunction(add_program PROG)

function(add_module MOD SRC)
  unset(INCLUDES)
  #[[ foreach(DEP ${ARGN})
    list(FIND MODSUBDIRS "${DEP}" I)
    if(I GREATER -1)
    list(APPEND INCLUDES "lib/${DEP}.h")
    endif(I GREATER -1)
    endforeach(DEP ${ARGN})]]
  # message("Program '${MOD}' dependencies: ${INCLUDES}")
  set(LIBS "")
  foreach(LIB ${ARGN})
    if(BUILD_SHARED_LIBS)
      list(APPEND LIBS "${LIB}-shared")
    endif(BUILD_SHARED_LIBS)
  endforeach(LIB ${ARGN})
  if(NOT EMSCRIPTEN)
    add_library(${MOD} SHARED ${SRC})
    target_link_libraries(${MOD} ${LIBS} ${EXTRA_LIBRARIES})
    install(TARGETS ${MOD} DESTINATION libexec)
    if(BUILD_SHARED_LIBS)
      set_target_properties(${MOD} PROPERTIES EXE_LINKER_FLAGS "-L${OPENSSL_LIB_DIR} -shared")
    endif(BUILD_SHARED_LIBS)
    target_compile_options(${MOD} PUBLIC -fPIC)
  endif(NOT EMSCRIPTEN)
endfunction(add_module MOD)

if(NOT LIBLZMA_LIBRARY)
  set(LIBLZMA_LIBRARY "${LIBLZMA_LIBRARY}")
endif(NOT LIBLZMA_LIBRARY)

set(COMPRESSION_LIBRARIES ${LIBLZMA_LIBRARY} ${ZLIB_LIBRARY} ${BZIP2_LIBRARY} ${BROTLI_LIBRARY} ${LIBMATH})
dump(COMPRESSION_LIBRARIES)
if(OPENSSL_LIBRARIES)
  if(LINK_STATIC)
    set(CRYPTO_LIBRARIES ${OPENSSL_STATIC_LIBRARIES} pthread ${LIBCOMPAT})
  else(LINK_STATIC)
    set(CRYPTO_LIBRARIES ${OPENSSL_LIBRARIES} pthread ${LIBCOMPAT})
  endif(LINK_STATIC)
endif(OPENSSL_LIBRARIES)

if(LINK_STATIC)
  message(STATUS "Static linking")
  if(NOT CMAKE_EXE_LINKER_FLAGS MATCHES "-static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static" CACHE STRING "Executable linker flags" FORCE)
  endif(NOT CMAKE_EXE_LINKER_FLAGS MATCHES "-static")
endif(LINK_STATIC)

add_program(binfmttest binfmt)
if(USE_BZIP)
  add_program(bsdiffcat path errmsg buffer array safemult str byte errmsg uint32 uint64 ${COMPRESSION_LIBRARIES})
  add_program(ccat path case buffer open str unix ${COMPRESSION_LIBRARIES})
endif(USE_BZIP)
add_program(buffertest stralloc buffer array case mmap ${COMPRESSION_LIBRARIES})
add_program(compiler-wrapper stralloc buffer byte fmt str strlist dir)
add_program(count-depth buffer fmt)
add_program(decode-ls-lR stralloc buffer byte fmt io open str)
add_program(dnsip dns stralloc buffer errmsg socket ${LIBIPHLPAPI})
add_program(dnsname dns stralloc buffer errmsg ${LIBIPHLPAPI})
add_program(dnstest dns stralloc buffer errmsg ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(dnsip6 dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(dnsip6q dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(dnsipq dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(dnsmx dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})
#[[add_program(dnsq dns case stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(dnsqr dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})]]
# add_program(dnstrace dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})
add_program(dnstxt dns stralloc buffer errmsg socket ${LIBIPHLPAPI} ${LIBMSWSOCK})

add_program(eagle-gen-cmds xml stralloc buffer strarray array byte cb cbmap errmsg fmt hmap mmap scan str unix ${LIBMATH})
add_program(eagle-init-brd xml stralloc buffer hmap strlist scan str ${LIBMATH})
add_program(eagle-to-circuit xml stralloc buffer strlist array byte cb cbmap fmt hashmap hmap mmap scan str ${LIBMATH})
add_program(eagle-to-svg xml stralloc buffer strlist array byte cb cbmap fmt hashmap hmap mmap scan str)
add_program(elf64list elf mmap uint16 uint32 uint64)
add_program(elflist buffer alloc errmsg unix elf mmap str)
add_program(elfwrsec buffer alloc elf mmap)
add_program(genmakefile sig set case errmsg strarray slist rdir dir path strlist hashmap hmap stralloc buffer mmap unix open scan fmt byte str array safemult alloc)
add_program(piccfg ihex errmsg strarray slist rdir dir path strlist hashmap hmap stralloc buffer mmap unix open scan fmt byte str array safemult alloc)
add_program(hexedit stralloc buffer array io errmsg mmap open path scan uint64 unix ${ZLIB_LIBRARY})
add_program(httptest http tls dns stralloc buffer errmsg byte io socket alloc taia ${LIBSSL} ${LIBMSWSOCK} ${LIBDL} pthread)
set(ssltest_SOURCES address.c address.h)
add_program(ssltest strlist unix tls dns stralloc buffer errmsg byte io socket alloc taia ${LIBSSL} ${LIBMSWSOCK} ${LIBDL} pthread)
add_program(impgen pe buffer byte mmap open)
add_program(jsontest json stralloc buffer byte charbuf fmt hmap mmap open)
add_program(list-r path strarray strlist stralloc buffer array unix dir array fmt open str uint64 ${LIBADVAPI32})
add_program(macho32list mmap uint32)
if(USE_LZMA)
  add_program(mediathek-list errmsg stralloc buffer alloc byte fmt http scan slist str unix strarray strlist ${LIBLZMA_LIBRARY} ${LIBSSL} ${LIBDL} pthread)
endif(USE_LZMA)

add_program(mediathek-parser case ucs errmsg strarray strlist stralloc buffer array byte open str unix strlist)
add_program(ntldd stralloc buffer pe strlist byte path str unix uint64)
add_program(opensearch-dump xml stralloc buffer)
add_program(pathtool stralloc buffer errmsg path strlist unix)
add_program(pelist buffer errmsg mmap pe str unix)
add_program(omflist stralloc buffer errmsg omf mmap str unix)
add_program(pkgcfg wait path unix hashmap hmap stralloc buffer byte cbmap dir env errmsg slist case str strarray strlist)
add_program(plsconv stralloc buffer byte open playlist str unix)
add_program(rdir-test rdir stralloc buffer)
add_program(reg2cmd stralloc buffer byte fmt open scan str uint64)
add_program(regfilter stralloc buffer byte fmt open scan str uint64)
add_program(sln unix stralloc buffer byte errmsg path)
add_program(pipeinfo slist seek unix stralloc buffer array byte errmsg path)
add_program(strarraytest xml dns stralloc buffer mmap strarray unix)
add_program(torrent-progress stralloc buffer io mmap open uint64)
add_program(xmlpp errmsg xml stralloc buffer hmap io unix)
add_program(xmltest xml stralloc buffer byte fmt hmap)
add_program(xmltest2 xml stralloc buffer byte fmt hmap)
add_program(xmltest3 xml stralloc buffer byte fmt hmap)
add_program(xmltest4 xml stralloc buffer array byte cb cbmap fmt hmap mmap scan str strlist)
# add_program(ziptest buffer str unix ${COMPRESSION_LIBRARIES})
add_program(cc-wrap errmsg env buffer path dir strarray strlist stralloc fmt)
add_program(ar-wrap path process errmsg env buffer dir strarray strlist stralloc fmt alloc)
add_program(cofflist coff unix errmsg stralloc buffer mmap)
add_program(msys-shell unix errmsg env buffer path strarray strlist stralloc)
set(tcping_SOURCES address.c)
add_program(tcping hmap unix dns case io array socket ndelay errmsg taia tai buffer stralloc mmap open fmt scan str byte uint16 ${LIBMSWSOCK} ${LIBIPHLPAPI})
set(ftptest_SOURCES address.c ftplib.c)
add_program(ftptest hmap unix dns case io array socket ndelay errmsg taia tai stralloc buffer mmap open fmt scan str byte uint16 ${LIBMSWSOCK} ${LIBIPHLPAPI})

set(nscache_SOURCES cache.c cache.h log.c log.h query.c query.h response.c response.h roots.c roots.h names.c names.h lib/alloc.h lib/array.h lib/buffer.h lib/byte.h lib/case.h lib/dir.h lib/dns.h lib/env.h lib/errmsg.h lib/fmt.h lib/iopause.h lib/ip4.h lib/ip6.h lib/mmap.h lib/ndelay.h lib/open.h lib/path.h lib/scan.h lib/socket.h lib/str.h lib/stralloc.h lib/tai.h lib/taia.h lib/uint16.h
                    lib/uint32.h lib/uint64.h)
add_program(nscache dns path errmsg dir array stralloc alloc env byte uint16 ${LIBMSWSOCK} ${LIBIPHLPAPI})
add_program(crc errmsg buffer mmap open ${ZLIB_LIBRARY})
add_program(cmake-run errmsg env buffer path dir strarray strlist stralloc fmt)

add_program(jsonpp json errmsg unix)
add_program(xml2json json xml errmsg unix)
add_program(xml2moon xml textcode errmsg)
add_program(strip-comments errmsg env charbuf buffer path dir strarray strlist stralloc fmt)
add_program(quote textcode unix errmsg env charbuf buffer path dir strarray strlist stralloc fmt)

add_program(httpproxy http io map)
add_program(testini ini.c ini.h hmap stralloc buffer mmap open uint16)
# add_program(montty io map)
add_program(logserial sig charbuf hmap path unix rdir dir errmsg io ndelay strarray stralloc buffer taia tai)
add_program(tinyproxy process unix path wait range dns taia tai hmap env ndelay buffer strarray array strlist stralloc errmsg sig socket fmt scan io uint32)
target_link_libraries(tinyproxy ${LIBMSWSOCK})
# if(NOT WIN32) add_program(tcpproxy http io errmsg buffer open ) endif(NOT WIN32) add_program(redir http io ${LIBCOMPAT})

add_program(parse strlist errmsg)
add_program(testihex ihex stralloc buffer alloc mmap)
add_program(setuidgid errmsg path stralloc mmap open scan str)
add_program(lsproc strlist errmsg mmap buffer unix dir)

#[[add_program(ftpcopy io)
add_program(ftpls io)

set(ftpcopy_SOURCES ftpparse.c ftplib.c ftp_read_lis.c)
set(ftpls_SOURCES ftpparse.c ftplib.c ftp_read_lis.c)]]

# add_program(libc-resolv-override dns errmsg  mmap open scan str)

if(ENABLE_SHARED AND BUILD_SHARED_LIBS)
  add_module(libresolvoverride libc-resolv-override.c dns stralloc)
endif(ENABLE_SHARED AND BUILD_SHARED_LIBS)
add_subdirectory(tests)
add_subdirectory(examples)
