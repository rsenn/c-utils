set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_WARN_VS8 OFF)

set(CMAKE_C_STANDARD 11)

project(c-utils C ASM)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "/usr/local"
      CACHE PATH "Installation prefix" FORCE)
endif()
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

option(USE_LZMA "lzma compression" ON)
option(USE_BZIP "bzip2 compression" ON)
option(USE_ZLIB "zlib compression" ON)
option(USE_SSL "ssl encryption" ON)

if(WIN32 OR WINDOWS)
  option(USE_SELECT "Use select() syscall" ON)
else(WIN32 OR WINDOWS)
  option(USE_SELECT "Use select() syscall" OFF)
endif(WIN32 OR WINDOWS)

#[[option(USE_POLL "Use poll() syscall" OFF)
option(USE_EPOLL "Use epoll() syscall" OFF)]]
option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(LINK_STATIC "Link libraries static" OFF)
option(ENABLE_WERROR "Enable -Werror (warnings are errors)" OFF)

macro(debug_option NAME DESC)
  option(DEBUG_${NAME} "${DESC}" OFF)
  if(DEBUG_${NAME})
    add_definitions(-DDEBUG_${NAME}=1)
  endif(DEBUG_${NAME})
endmacro(
  debug_option
  NAME
  DESC)

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
endif(LINK_STATIC)

set(DIRLIST_VERSION 1.0)
set(CPACK_SOURCE_GENERATOR TXZ)
set(CPACK_SOURCE_IGNORE_FILES ${ignored_files})
set(CPACK_SOURCE_PACKAGE_FILE_NAME dirlist-${DIRLIST_VERSION})
set(CPACK_PACKAGE_NAME dirlist)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)
set(CPACK_INSTALL_CMAKE_PROJECTS
    ${CMAKE_CURRENT_BINARY_DIR};${CMAKE_PROJECT_NAME};ALL)

# if(EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
# add_subdirectory(3rdparty/openssl) endif()

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
include(CheckSymbolExists)

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
    string(
      RANDOM
      LENGTH 6
      ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
               C_NAME)
    string(REPLACE SUPPORT_ "" NAME "${RESULT_VAR}")
    string(REPLACE _ - NAME "${NAME}")
    string(TOLOWER "${NAME}" C_NAME)
    set(C_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/try-${C_NAME}.c")
    string(REPLACE "\\" "\\\\" SOURCE "${SOURCE}")
    file(WRITE "${C_SOURCE}" "${SOURCE}")
    message(STATUS "Trying to compile try-${C_NAME}.c ... ")
    try_compile(
      COMPILE_RESULT "${CMAKE_CURRENT_BINARY_DIR}"
      "${C_SOURCE}"
      OUTPUT_VARIABLE "OUTPUT"
      LINK_LIBRARIES "${ARGN}")
    file(REMOVE "${C_SOURCE}")

    if(COMPILE_RESULT)
      message(STATUS "ok")
      # add_definitions(-D${RESULT_VAR})
    else(COMPILE_RESULT)
      set(COMPILE_LOG "${CMAKE_CURRENT_BINARY_DIR}/compile-${C_NAME}.log")
      message(STATUS "fail: ${COMPILE_LOG}")
      file(WRITE "${COMPILE_LOG}" "${OUTPUT}")
      string(REPLACE "\n" ";" OUTPUT "${OUTPUT}")
      list(FILTER OUTPUT INCLUDE REGEX "error")
    endif(COMPILE_RESULT)

    set("${RESULT_VAR}"
        "${COMPILE_RESULT}"
        CACHE BOOL "Support ${NAME}")
  endif(RESULT STREQUAL "")
  show_result(${RESULT_VAR})
endmacro(
  check_compile
  RESULT_VAR
  SOURCE)
macro(check_run RESULT_VAR SOURCE)
  string(
    RANDOM
    LENGTH 6
    ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
             C_SOURCE)
  set(C_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/try-${C_SOURCE}.c")
  string(REPLACE "\\" "\\\\" SOURCE "${SOURCE}")
  file(WRITE "${C_SOURCE}" "${SOURCE}")
  try_run(
    RUN_RESULT COMPILE_RESULT "${CMAKE_CURRENT_BINARY_DIR}" "${C_SOURCE}"
    RUN_OUTPUT_VARIABLE RUN_OUTPUT
    COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT)

  if("${RUN_RESULT}" STREQUAL 0)
    set(RUN_OK TRUE)
  else("${RUN_RESULT}" STREQUAL 0)
    set(RUN_OK FALSE)
  endif("${RUN_RESULT}" STREQUAL 0)

  # message("Run result: ${RUN_OK} output: ${RUN_OUTPUT}") message("Compile
  # result: ${COMPILE_RESULT}")

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
    set("${ARGN}"
        "${RUN_OUTPUT}"
        PARENT_SCOPE)
  endif(ARGN)

  show_result(${RESULT_VAR})
endmacro(
  check_run
  RESULT_VAR
  SOURCE)

if(MINGW
   OR WIN32
   OR WIN64
   OR MSVC)
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
endif(
  MINGW
  OR WIN32
  OR WIN64
  OR MSVC)

check_compile(
  LIBC_HAS_IP6
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\n  struct sockaddr_in6 sa;\n  sa.sin6_family = PF_INET6;\n  (void)sa;\n  return 0;\n}\n"
)
check_compile(
  LIBC_HAS_SCOPE_ID
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\n  struct sockaddr_in6 sa;\n  sa.sin6_family = PF_INET6;\n  sa.sin6_scope_id = 23;\n  (void)sa;\n  return 0;\n}\n"
)
check_compile(
  HAVE_SYSCALL
  "#define _GNU_SOURCE\n#include <unistd.h>\n#include <sys/syscall.h>\n\nstatic inline ssize_t\nsys_write(int fd, const void* buf, size_t n) {\n  return syscall(SYS_write, fd, buf, n);\n}\n\nint main() { return sys_write(1, \"test\\\\n\", 5);\n}\n"
)

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
    "#include <sys/types.h>\n#include <fcntl.h>\n#include <unistd.h>\n#include <poll.h>\n\nint main() { struct pollfd x;\n\n  x.fd = open(\"trypoll.c\", O_RDONLY);\n  if(x.fd == -1)\n    _exit(111);\n  x.events = POLLIN;\n  if(poll(&x, 1, 10) == -1)\n    _exit(1);\n  if(x.revents != POLLIN)\n    _exit(1);\n\n  /* XXX: try to detect and avoid poll() imitation libraries */\n\n  _exit(0);\n}\n"
  )
  check_compile(
    SUPPORT_SELECT
    "#include <sys/types.h>\n#include <sys/time.h>\n#if defined(_WIN32) || defined(__MINGW32__)\n#include <winsock2.h>\n#else\n#include <sys/select.h>\n#include <unistd.h>\n#endif\n\nint\nmain() {\n  struct timeval tv;\n  fd_set rfds;\n  int fd = 0;\n  FD_ZERO(&rfds);\n  FD_SET(fd, &rfds);\n  tv.tv_sec = tv.tv_usec = 0;\n  select(fd + 1, &rfds, 0, 0, &tv);\n}\n"
    ${LIBMSWSOCK})

  function(LIST_CONTAINS LIST ITEM OUTPUT_VAR)
    set(RESULT FALSE)
    foreach(IT ${LIST})
      if("${IT}" STREQUAL "${ITEM}")
        set(RESULT TRUE)
      endif("${IT}" STREQUAL "${ITEM}")
    endforeach(IT ${LIST})
    set("${OUTPUT_VAR}"
        ${RESULT}
        PARENT_SCOPE)
  endfunction(
    LIST_CONTAINS
    LIST
    ITEM
    OUTPUT_VAR)

endif(NOT USE_SELECT)

check_compile(
  HAVE_N2I
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <net/if.h>\n\nint main() { static char ifname[IFNAMSIZ];\n  char* tmp = if_indextoname(0, ifname);\n  (void)tmp;\n  return 0;\n}\n"
)

check_compile(
  HAVE_BSDSENDFILE
  "/* for macos X, don't ask */\n#define SENDFILE 1\n\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/uio.h>\n#include <errno.h>\n#include <netinet/in.h>\n#include <netinet/tcp.h>\n\nint main() { struct sf_hdtr hdr;\n  struct iovec v[17 + 23];\n  int r, fd = 1;\n  off_t sbytes;\n  hdr.headers = v;\n  hdr.hdr_cnt = 17;\n  hdr.trailers = v + 17;\n  hdr.trl_cnt = 23;\n  r = sendfile(0, 1, 37, 42, &hdr, &sbytes, 0);\n  (void)r;\n  return 0;\n}\n"
)

check_compile(
  HAVE_SENDFILE
  "#ifdef __hpux__\n#include <sys/types.h>\n#include <sys/uio.h>\n#include <sys/socket.h>\n#include <stdio.h>\n\nint main() { /*\n        sbsize_t sendfile(int s, int fd, off_t offset, bsize_t nbytes,\n                      const struct iovec *hdtrl, int flags);\n  */\n  struct iovec x[2];\n  int fd = open(\"havesendfile.c\", 0);\n  x[0].iov_base = \"header\";\n  x[0].iov_len = 6;\n  x[1].iov_base = \"footer\";\n  x[1].iov_len = 6;\n  sendfile(1 /* dest socket */, fd /* src file */, 0 /* offset */, 23 /* nbytes */, x, 0);\n  perror(\"sendfile\");\n  return 0;\n}\n#elif defined(__sun__) && defined(__svr4__)\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/sendfile.h>\n#include <stdio.h>\n\nint main() { off_t o;\n  o = 0;\n  sendfile(1 /* dest */, 0 /* src */, &o, 23 /* nbytes */);\n  perror(\"sendfile\");\n  return 0;\n}\n#elif defined(_AIX)\n\n#define _FILE_OFFSET_BITS 64\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <unistd.h>\n#include <stdio.h>\n\nint main() { int fd = open(\"havesendfile.c\", 0);\n  struct sf_parms p;\n  int destfd = 1;\n  p.header_data = \"header\";\n  p.header_length = 6;\n  p.file_descriptor = fd;\n  p.file_offset = 0;\n  p.file_bytes = 23;\n  p.trailer_data = \"footer\";\n  p.trailer_length = 6;\n  if(send_file(&destfd, &p, 0) >= 0)\n    printf(\"sent %lu bytes.\\\\n\", p.bytes_sent);\n  return 0;\n}\n#elif defined(__linux__)\n\n#define _FILE_OFFSET_BITS 64\n#include <sys/types.h>\n#include <unistd.h>\n#if defined(__GLIBC__)\n#include <sys/sendfile.h>\n#elif defined(__dietlibc__)\n#include <sys/sendfile.h>\n#else\n#include <linux/unistd.h>\n_syscall4(int, sendfile, int, out, int, in, long*, offset, unsigned long, count)\n#endif\n#include <stdio.h>\n\nint main() { int fd = open(\"havesendfile.c\", 0);\n  off_t o = 0;\n  off_t r = sendfile(1, fd, &o, 23);\n  if(r != -1)\n    printf(\"sent %llu bytes.\\\\n\", r);\n  return 0;\n}\n\n#else\n#error unsupported architecture\n/* stupid fucking IRIX c99 does not signal #error via a non-zero exit\n * code!  ARGH!  So insert a parse error */\n#include rumpelstilzchen\n)\n#endif\n"
)

check_compile(
  HAVE_SOCKLEN_T
  "#include <sys/types.h>\n#ifdef __MINGW32__\n#include <winsock2.h>\n#include <ws2tcpip.h>\n#else\n#include <sys/socket.h>\n#include <netinet/in.h>\n#endif\n\nint main() { socklen_t t;\n  (void)t;\n  return 0;\n}\n"
)
if(NOT HAVE_SOCKLEN_T)
  add_definitions(-Dsocklen_t=int)
endif(NOT HAVE_SOCKLEN_T)

check_compile(
  HAVE___INLINE__
  "static __inline__ int\nfoo(int bar) {\n  return bar + 1;\n}\nint main() { return foo(42); }\n"
)
if(HAVE___INLINE__)
  add_definitions(-Dinline=__inline)
else(HAVE___INLINE__)
  check_compile(
    HAVE___INLINE
    "static __inline int\nfoo(int bar) {\n  return bar + 1;\n}\nint main() { return foo(42); }\n"
  )
  if(HAVE___INLINE)
    add_definitions(-Dinline=__inline)
  else(HAVE___INLINE)
    check_compile(
      HAVE_INLINE
      "static inline int\nfoo(int bar) {\n  return bar + 1;\n}\nint main() { return foo(42); }\n"
    )
    if(NOT HAVE_INLINE)
      add_definitions(-Dinline=)
    endif(NOT HAVE_INLINE)
  endif(HAVE___INLINE)
endif(HAVE___INLINE__)

check_compile(HAVE_UINT128 "int main() { __uint128_t tmp; }\n")
check_compile(HAVE_INT128 "int main() { __int128_t tmp; }\n")

check_compile(
  HAVE_ALLOCA
  "#include <stdlib.h>\n#ifdef A\n#include <alloca.h>\n#else\n#include <malloc.h>\n#endif\n\nint main() { char* c = alloca(23);\n  (void)c;\n  return 0;\n}\n"
)

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

message("extra includes = ${CMAKE_EXTRA_INCLUDE_FILES}")

if(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CROSS FALSE)
else(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_TOOLCHAIN_FILE)
  set(CROSS TRUE)
endif(NOT CMAKE_CROSSCOMPILING AND NOT CMAKE_TOOLCHAIN_FILE)

if(NOT CROSS)
  check_type_size(sigset_t SIGSET_T_SIZE)
endif(NOT CROSS)
# message("CROSS = ${CROSS}")

# check_symbol_exists(__sync_val_compare_and_swap ""
# HAVE___SYNC_VAL_COMPARE_AND_SWAP)

check_compile(
  INTRINSIC_CAS
  "#include <stdlib.h>\n\nint main() {\n  long v;\n  __sync_val_compare_and_swap(&v, 1, 0);\n  return 0;\n}"
)

if(NOT INTRINSIC_CAS)
  add_definitions(-DUSE_INLINE_COMPARE_AND_SWAP=1)
endif()


function(DUMP VAR)
  string(REGEX REPLACE "[;\n]" " " A "${ARGV}")
  message("\n\nVariable dump of: ${A}\n")

  foreach(VAR ${ARGV})
    message("\t${VAR} = ${${VAR}}")
  endforeach(VAR ${ARGV})
  message("\n")
endfunction(DUMP VAR)


#DUMP(CMAKE_C_COMPILER_ID)

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
  set(HAVE_SIGSET_TYPE
      TRUE
      CACHE BOOL "Define this if you have the sigset_t type")
else(SIGSET_T_SIZE)
  check_symbol_exists(sigset_t "signal.h" HAVE_SIGSET_T)
endif(SIGSET_T_SIZE)

if(NOT HAVE_SIGSET_T OR NOT SIZEOF_SIGSET_T)

  check_run(
    HAVE_SIGSET_T
    "#define _GNU_SOURCE\n#include <sys/types.h>\n#include <signal.h>\n#include <fcntl.h>\n#include <stdio.h>\n\nint main() {\n  sigset_t ss;\nprintf(\"%lu\\\\n\", sizeof(ss));\n  return 0;\n}\n"
    SIZEOF_SIGSET_T)
  # message("SIZEOF_SIGSET_T = ${SIZEOF_SIGSET_T}")
endif(NOT HAVE_SIGSET_T OR NOT SIZEOF_SIGSET_T)

if(HAVE_SIGSET_T)
  # add_definitions(-DHAVE_SIGSET_T)
endif(HAVE_SIGSET_T)

# set(SIZEOF_SIGSET_TYPE "${SIZEOF_SIGSET_T}" CACHE STRING "Size of 'sigset_t'
# type")
message("size of sigset_t = ${SIZEOF_SIGSET_T}")

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
  #check_type_size(int16_t INT16_T)
  #check_type_size(int32_t INT32_T)
  #check_type_size(int64_t INT64_T)
  #check_type_size(intmax_t INTMAX_T)
  #check_type_size(uint8_t UINT8_T)
  #check_type_size(uint16_t UINT16_T)
  #check_type_size(uint32_t UINT32_T)
  #check_type_size(uint64_t UINT64_T)
  #check_type_size(uintmax_t UINTMAX_T)


  #check_type_size(ptrdiff_t PTRDIFF_T_SIZE)
  #check_type_size(size_t SIZE_T_SIZE)
  #check_type_size(long LONG_SIZE)
  #check_type_size("void*" VOIDP_T_SIZE)

  #DUMP(PTRDIFF_T_SIZE)
  #DUMP(SIZE_T_SIZE)
  #DUMP(LONG_SIZE)
  #DUMP(VOIDP_T_SIZE)

  #check_type_size("short" SIZE_OF_SHORT)
  #check_type_size("int" SIZE_OF_INT)
  #check_type_size("long" SIZE_OF_LONG)
  #check_type_size("long long" SIZE_OF_LONG_LONG)

  #check_type_size("unsigned short" SIZE_OF_UNSIGNED_SHORT)
  #check_type_size("unsigned" SIZE_OF_UNSIGNED)
  #check_type_size("unsigned long" SIZE_OF_UNSIGNED_LONG)
  #check_type_size("unsigned long long" SIZE_OF_UNSIGNED_LONG_LONG)
  #check_type_size("size_t" SIZE_OF_SIZE_T)

  #check_type_size("__int64" __INT64)
  #check_type_size("unsigned __int64" UNSIGNED___INT64)

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
  dump(EMSCRIPTEN EMCC_SUFFIX EMSCRIPTEN_PREFIX EMSCRIPTEN_TOOLCHAIN_PATH
       COMPILER_NAME CMAKE_EXE_LINKER_FLAGS)
  # foreach(LIB BZIP ZLIB LZMA SSL) set("USE_${LIB}" OFF) endforeach(LIB BZIP
  # ZLIB LZMA SSL)
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

if(NOT LINK_STATIC STREQUAL OFF)
  set(EXE_LINKER_FLAGS "${EXE_LINKER_FLAGS} -static")
  set(CHK_DLFCN FALSE)
endif(NOT LINK_STATIC STREQUAL OFF)
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
endmacro(
  check_function_def
  FUNC
  RESULT_VAR)
function(check_functions)
  foreach(FUNC ${ARGN})
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
    check_function_def("${FUNC}" "${RESULT_VAR}")
  endforeach(FUNC ${ARGN})
endfunction(check_functions)

check_functions(
  lstat
  waitpid
  aligned_alloc
  fnmatch
  getdelim
  getopt
  getopt_long
  pipe2
  popen
  posix_memalign
  pread
  sendfile
  sigaddset
  sigemptyset
  sigprocmask
  strerror_r
  __write
  writev
  posix_spawnattr_setflags
  posix_spawn_file_actions_init
  posix_spawn_file_actions_adddup2
  posix_spawnp)
if(HAVE_POSIX_SPAWNATTR_SETFLAGS
   AND HAVE_POSIX_SPAWN_FILE_ACTIONS_INIT
   AND HAVE_POSIX_SPAWN_FILE_ACTIONS_ADDDUP2
   AND HAVE_POSIX_SPAWNP)
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

if(HAVE_OPENDIR
   AND HAVE_READDIR
   AND HAVE_CLOSEDIR)
  add_definitions(-DUSE_READDIR=1)
endif(
  HAVE_OPENDIR
  AND HAVE_READDIR
  AND HAVE_CLOSEDIR)

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

# message(INFO "Checking for epoll()...") check_function_exists(epoll_wait
# SUPPORT_EPOLL) if(SUPPORT_EPOLL) message(INFO "Checking for epoll()... OK")
# set(USE_EPOLL ON) set(USE_SELECT OFF) else(SUPPORT_EPOLL) message(INFO
# "Checking for epoll()... NOT FOUND") set(USE_EPOLL OFF) endif(SUPPORT_EPOLL)

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

# include(${CMAKE_SOURCE_DIR}/FindZlib.cmake)
# include(${CMAKE_SOURCE_DIR}/FindBZip2.cmake)
# include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)

include(${CMAKE_SOURCE_DIR}/build/cmake/CheckInline.cmake)
include(${CMAKE_SOURCE_DIR}/build/cmake/Profile.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  debug_option(OUTPUT "Output verbose messages")
  add_definitions(-D_DEBUG=1)

  if(DEBUG_OUTPUT)
    add_definitions(-DDEBUG_OUTPUT=1)
  endif(DEBUG_OUTPUT)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                  CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_MINSIZEREL)
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
  set(${CompilerFlag}
      "${FLAG}"
      CACHE STRING "Compiler flags ${CompilerFlag}")
  # message("${CompilerFlag}: ${${CompilerFlag}}")
endforeach()

include(${CMAKE_SOURCE_DIR}/build/cmake/Werror.cmake)

if(USE_LZMA)
  # ############################################################################
  # Check LibLZMA
  # ############################################################################
  # pkg_search_module(LIBLZMA liblzma lzma) if(LIBLZMA_FOUND)
  # if(pkgcfg_lib_LIBLZMA_lzma) set(LIBLZMA_LIBRARY ${pkgcfg_lib_LIBLZMA_lzma}
  # CACHE FILEPATH "") endif(pkgcfg_lib_LIBLZMA_lzma) endif(LIBLZMA_FOUND)
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

  # ############################################################################
  # Check Zlib
  # ############################################################################
  message(STATUS "Checking for libz")
  find_library(ZLIB z)

  message(STATUS "${ZLIB}")

  set(ZLIB_LIBRARY "${ZLIB}")

  if(ZLIB)
    set(ZLIB_FOUND TRUE)
    if(ZLIB STREQUAL "")
      set(ZLIB z)
    endif(ZLIB STREQUAL "")
    set(ZLIB_LIBARY_RELEASE
        "${ZLIB}"
        CACHE FILEPATH "")
    set(ZLIB_LIBARY_DEBUG
        "${ZLIB}"
        CACHE FILEPATH "")
    set(ZLIB_LIBARY
        "${ZLIB}"
        CACHE FILEPATH "")
  endif(ZLIB)

  if(NOT ZLIB_FOUND)

    pkg_search_module(ZLIB zlib libz z)
    if(ZLIB_FOUND)
      if(pkgcfg_lib_ZLIB_z)
        set(ZLIB_LIBRARY_RELEASE
            "${pkgcfg_lib_ZLIB_z}"
            CACHE FILEPATH "")
        set(ZLIB_LIBRARY_DEBUG
            "${pkgcfg_lib_ZLIB_z}"
            CACHE FILEPATH "")
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
  # ############################################################################
  # Check BZip2
  # ############################################################################
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
    endif(BZIP2_LIBRARY)

  else(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
    message(STATUS "bzip2 package found: ${BZIP2_PREFIX} ${BZIP2_LIBRARY_DIRS} ${BZIP2_LIBRARIES}")
    #pkg_get_variable(BZIP2_LIBRARY "${BZIP2_MODULE_NAME}")
    set(BZIP2_LIBRARY ${BZIP2_LIBRARIES})
    set(BZIP2_LINK_LIBRARY ${BZIP2_LINK_LIBRARIES})
    set(BZIP2_INCLUDE_DIR ${BZIP2_INCLUDE_DIRS})
    set(BZIP2_LIBRARY_DIR ${BZIP2_LIBRARY_DIRS})
  endif(NOT BZIP2_FOUND OR "${BZIP2_FOUND}" STREQUAL "")
endif(USE_BZIP)

# ##############################################################################
# Check OpenSSL
# ##############################################################################
if(USE_SSL AND NOT BUILD_SSL)
  # message("OPENSSL_ROOT = $ENV{OPENSSL_ROOT}")
  if(NOT OPENSSL_ROOT_DIR)
    if("$ENV{OPENSSL_ROOT}")
      set(OPENSSL_ROOT_DIR
          "$ENV{OPENSSL_ROOT}"
          CACHE FILEPATH "OpenSSL root directory")
    else("$ENV{OPENSSL_ROOT}")
      set(OPENSSL_ROOT_DIR
          "${CMAKE_INSTALL_PREFIX}"
          CACHE FILEPATH "OpenSSL root directory")
    endif("$ENV{OPENSSL_ROOT}")
  endif(NOT OPENSSL_ROOT_DIR)

  set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH OFF)
  pkg_search_module(OPENSSL openssl libssl NO_CMAKE_PATH)

  if(OPENSSL_FOUND)
    check_include_file(openssl/ssl.h HAVE_OPENSSL_SSL_H)
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

  message(
    STATUS
      "OPENSSL_FOUND=${OPENSSL_FOUND}\n   OPENSSL_INCLUDE_DIR=${OPENSSL_LIB_DIR}\n   OPENSSL_LIB_DIR=${OPENSSL_INCLUDE_DIR}\n   OPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}\n   OPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}\n   OPENSSL_SSL_LIBRARY=${OPENSSL_SSL_LIBRARY}"
  )

else()
  unset(OPENSSL_LIBRARIES CACHE)
  unset(OPENSSL_CRYPTO_LIBRARY CACHE)
  unset(OPENSSL_SSL_LIBRARY CACHE)
  unset(OPENSSL_INCLUDE_DIR CACHE)
endif()
# link_libraries( ${ZLIB_LIBRARY} ${BZIP2_LIBRARY} ${LIBLZMA_LIBRARY}
# ${CRYPTO_LIBRARIES} ) add_definitions(-Dinline=__inline)

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
set(MUX_METHODS
    "${MUX_LIST}"
    CACHE STRING "available I/O multiplexing methods")

message("Available MUX methods: ${MUX_METHODS}")

# message("MUX definitions: ${MUX_DEFS}")

if(LIBMSWSOCK)
  list(APPEND CMAKE_REQUIRED_LIBRARIES ${LIBMSWSOCK})
endif(LIBMSWSOCK)

set(MUX_METHOD
    "${MUX}"
    CACHE STRING "I/O Multiplexer method")
message("MUX method: ${MUX_METHOD}")

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
  # message("ZLIB_LIBRARY: ${ZLIB_LIBRARY} ZLIB_FOUND: ${ZLIB_FOUND}")
  # if(ZLIB_INCLUDE_DIR) link_libraries(${ZLIB_LIBRARY})
  include_directories(${ZLIB_INCLUDE_DIR})
  # endif(ZLIB_INCLUDE_DIR)

  # if(ZLIB_LIBRARY)
  add_definitions(-DHAVE_ZLIB=1)
  # else(ZLIB_LIBRARY) add_definitions(-DHAVE_ZLIB=0) endif(ZLIB_LIBRARY)
endif(USE_ZLIB)

if(USE_BZIP)
  # message("BZIP2_LIBRARY: ${BZIP2_LIBRARY} BZIP2_FOUND: ${BZIP2_FOUND}")
  # if(BZIP2_LIBRARY)
  add_definitions(-DHAVE_LIBBZ2=1)
  include_directories(${BZIP2_INCLUDE_DIR})
  # else(BZIP2_LIBRARY) add_definitions(-DHAVE_LIBBZ2=0) endif(BZIP2_LIBRARY)
endif(USE_BZIP)

if(USE_LZMA)
  # message("LIBLZMA_LIBRARY: ${LIBLZMA_LIBRARY} LIBLZMA_FOUND:
  # ${LIBLZMA_FOUND}") if(LIBLZMA_INCLUDE_DIR)
  # link_libraries(${LIBLZMA_LIBRARY})
  include_directories(${LIBLZMA_INCLUDE_DIR})
  # endif(LIBLZMA_INCLUDE_DIR) if(LIBLZMA_LIBRARY)
  add_definitions(-DHAVE_LIBLZMA=1 -DLZMA_API_STATIC=1)
  # else(LIBLZMA_LIBRARY) add_definitions(-DHAVE_LIBLZMA=0)
  # endif(LIBLZMA_LIBRARY)
endif(USE_LZMA)

macro(invert INVAR OUTVAR)
  if(${${INVAR}})
    set("${OUTVAR}" FALSE)
  else(${${INVAR}})
    set("${OUTVAR}" TRUE)
  endif(${${INVAR}})
endmacro(
  invert
  INVAR
  OUTVAR)

macro(invoption VAR DESC INVAR)
  if(${${INVAR}})
    option("${VAR}" "${DESC}" OFF)
  else(${${INVAR}})
    option("${VAR}" "${DESC}" ON)
  endif(${${INVAR}})
endmacro(
  invoption
  VAR
  DESC
  INVAR)

invoption(BUILD_LZMA "build 3rdparty liblzma" LIBLZMA_FOUND)
invoption(BUILD_BZIP "build 3rdparty libbz2" BZIP2_FOUND)
invoption(BUILD_ZLIB "build 3rdparty libz" ZLIB_FOUND)
invoption(BUILD_SSL "build 3rdparty openssl" OPENSSL_FOUND)

if(BUILD_ZLIB)
  message("Building zlib from 3rdparty/zlib")
  set(ZLIB_SOURCES
      3rdparty/zlib/adler32.c
      3rdparty/zlib/compress.c
      3rdparty/zlib/crc32.c
      3rdparty/zlib/deflate.c
      3rdparty/zlib/infback.c
      3rdparty/zlib/inffast.c
      3rdparty/zlib/inflate.c
      3rdparty/zlib/inftrees.c
      3rdparty/zlib/trees.c
      3rdparty/zlib/uncompr.c
      3rdparty/zlib/zutil.c)
  add_library(z ${ZLIB_SOURCES})
  set_target_properties(
    z PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/zlib"
                 LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/zlib")
  link_directories(BEFORE "${CMAKE_BINARY_DIR}/zlib")
  set(ZLIB_LIBRARY_DEBUG
      z
      CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY_RELEASE
      z
      CACHE FILEPATH "" FORCE)
  set(ZLIB_LIBRARY
      z
      CACHE FILEPATH "" FORCE)
  set(ZLIB_INCLUDE_DIR
      ${CMAKE_SOURCE_DIR}/3rdparty/zlib
      CACHE PATH "" FORCE)
endif(BUILD_ZLIB)

if(BUILD_BZIP)
  message("Building libbz2 from 3rdparty/bzip2")
  set(LIBBZ2_SOURCES
      3rdparty/bzip2/blocksort.c
      3rdparty/bzip2/bzlib.c
      3rdparty/bzip2/compress.c
      3rdparty/bzip2/crctable.c
      3rdparty/bzip2/decompress.c
      3rdparty/bzip2/huffman.c
      3rdparty/bzip2/randtable.c)
  add_library(bz2 ${LIBBZ2_SOURCES})
  set_target_properties(
    bz2 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bzip2"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bzip2")
  link_directories(BEFORE "${CMAKE_BINARY_DIR}/bzip2")

  set(BZIP2_LIBRARY_DEBUG
      bz2
      CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY_RELEASE
      bz2
      CACHE FILEPATH "" FORCE)
  set(BZIP2_LIBRARY
      bz2
      CACHE FILEPATH "" FORCE)
  set(BZIP2_INCLUDE_DIR
      ${CMAKE_SOURCE_DIR}/3rdparty/bzip2
      CACHE PATH "" FORCE)
endif(BUILD_BZIP)

function(DEFINE_HAVE OUTVAR)
  set(OUT "")
  foreach(DEF ${ARGN})
    # message("Check definition ${DEF}: ${${DEF}}")
    if(${DEF} STREQUAL 1)
      list(APPEND OUT "${DEF}=1")
    endif(${DEF} STREQUAL 1)
  endforeach(DEF ${ARGN})
  set("${OUTVAR}"
      "${OUT}"
      PARENT_SCOPE)

endfunction(DEFINE_HAVE)

if(BUILD_LZMA)
  message("Building liblzma from 3rdparty/xz")
  # file(WRITE "${CMAKE_BINARY_DIR}/lzma-config.h.cmake" "\n/*\n * Ensure we
  # have C99-style int64_t, etc, all defined.\n */\n\n/* First, we need to know
  # if the system has already defined them. */\n#cmakedefine
  # HAVE_INT16_T\n#cmakedefine HAVE_INT32_T\n#cmakedefine
  # HAVE_INT64_T\n#cmakedefine HAVE_INTMAX_T\n\n#cmakedefine
  # HAVE_UINT8_T\n#cmakedefine HAVE_UINT16_T\n#cmakedefine
  # HAVE_UINT32_T\n#cmakedefine HAVE_UINT64_T\n#cmakedefine HAVE_UINTMAX_T\n\n/*
  # We might have the types we want under other spellings. */\n#cmakedefine
  # HAVE___INT64\n#cmakedefine HAVE_U_INT64_T\n#cmakedefine
  # HAVE_UNSIGNED___INT64\n\n/* The sizes of various standard integer types.
  # */\n@SIZE_OF_SHORT_CODE@\n@SIZE_OF_INT_CODE@\n@SIZE_OF_LONG_CODE@\n@SIZE_OF_LONG_LONG_CODE@\n@SIZE_OF_UNSIGNED_SHORT_CODE@\n@SIZE_OF_UNSIGNED_CODE@\n@SIZE_OF_UNSIGNED_LONG_CODE@\n@SIZE_OF_UNSIGNED_LONG_LONG_CODE@\n@SIZE_OF_SIZE_T_CODE@\n\n/*\n
  # * If we lack int64_t, define it to the first of __int64, int, long, and long
  # long\n * that exists and is the right size.\n */\n#if !defined(HAVE_INT64_T)
  # && defined(HAVE___INT64)\ntypedef __int64 int64_t;\n#define
  # HAVE_INT64_T\n#endif\n\n#if !defined(HAVE_INT64_T) && SIZE_OF_INT ==
  # 8\ntypedef int int64_t;\n#define HAVE_INT64_T\n#endif\n\n#if
  # !defined(HAVE_INT64_T) && SIZE_OF_LONG == 8\ntypedef long int64_t;\n#define
  # HAVE_INT64_T\n#endif\n\n#if !defined(HAVE_INT64_T) && SIZE_OF_LONG_LONG ==
  # 8\ntypedef long long int64_t;\n#define HAVE_INT64_T\n#endif\n\n#if
  # !defined(HAVE_INT64_T)\n#error No 64-bit integer type was
  # found.\n#endif\n\n/*\n * Similarly for int32_t\n */\n#if
  # !defined(HAVE_INT32_T) && SIZE_OF_INT == 4\ntypedef int int32_t;\n#define
  # HAVE_INT32_T\n#endif\n\n#if !defined(HAVE_INT32_T) && SIZE_OF_LONG ==
  # 4\ntypedef long int32_t;\n#define HAVE_INT32_T\n#endif\n\n#if
  # !defined(HAVE_INT32_T)\n#error No 32-bit integer type was
  # found.\n#endif\n\n/*\n * Similarly for int16_t\n */\n#if
  # !defined(HAVE_INT16_T) && SIZE_OF_INT == 2\ntypedef int int16_t;\n#define
  # HAVE_INT16_T\n#endif\n\n#if !defined(HAVE_INT16_T) && SIZE_OF_SHORT ==
  # 2\ntypedef short int16_t;\n#define HAVE_INT16_T\n#endif\n\n#if
  # !defined(HAVE_INT16_T)\n#error No 16-bit integer type was
  # found.\n#endif\n\n/*\n * Similarly for uint64_t\n */\n#if
  # !defined(HAVE_UINT64_T) && defined(HAVE_UNSIGNED___INT64)\ntypedef unsigned
  # __int64 uint64_t;\n#define HAVE_UINT64_T\n#endif\n\n#if
  # !defined(HAVE_UINT64_T) && SIZE_OF_UNSIGNED == 8\ntypedef unsigned
  # uint64_t;\n#define HAVE_UINT64_T\n#endif\n\n#if !defined(HAVE_UINT64_T) &&
  # SIZE_OF_UNSIGNED_LONG == 8\ntypedef unsigned long uint64_t;\n#define
  # HAVE_UINT64_T\n#endif\n\n#if !defined(HAVE_UINT64_T) &&
  # SIZE_OF_UNSIGNED_LONG_LONG == 8\ntypedef unsigned long long
  # uint64_t;\n#define HAVE_UINT64_T\n#endif\n\n#if
  # !defined(HAVE_UINT64_T)\n#error No 64-bit unsigned integer type was
  # found.\n#endif\n\n/*\n * Similarly for uint32_t\n */\n#if
  # !defined(HAVE_UINT32_T) && SIZE_OF_UNSIGNED == 4\ntypedef unsigned
  # uint32_t;\n#define HAVE_UINT32_T\n#endif\n\n#if !defined(HAVE_UINT32_T) &&
  # SIZE_OF_UNSIGNED_LONG == 4\ntypedef unsigned long uint32_t;\n#define
  # HAVE_UINT32_T\n#endif\n\n#if !defined(HAVE_UINT32_T)\n#error No 32-bit
  # unsigned integer type was found.\n#endif\n\n/*\n * Similarly for uint16_t\n
  # */\n#if !defined(HAVE_UINT16_T) && SIZE_OF_UNSIGNED == 2\ntypedef unsigned
  # uint16_t;\n#define HAVE_UINT16_T\n#endif\n\n#if !defined(HAVE_UINT16_T) &&
  # SIZE_OF_UNSIGNED_SHORT == 2\ntypedef unsigned short uint16_t;\n#define
  # HAVE_UINT16_T\n#endif\n\n#if !defined(HAVE_UINT16_T)\n#error No 16-bit
  # unsigned integer type was found.\n#endif\n\n/*\n * Similarly for uint8_t\n
  # */\n#if !defined(HAVE_UINT8_T)\ntypedef unsigned char uint8_t;\n#define
  # HAVE_UINT8_T\n#endif\n\n#if !defined(HAVE_UINT16_T)\n#error No 8-bit
  # unsigned integer type was found.\n#endif\n\n/* Define intmax_t and uintmax_t
  # if they are not already defined. */\n#if !defined(HAVE_INTMAX_T)\ntypedef
  # int64_t intmax_t;\n#define INTMAX_MIN INT64_MIN\n#define INTMAX_MAX
  # INT64_MAX\n#endif\n\n#if !defined(HAVE_UINTMAX_T)\ntypedef uint64_t
  # uintmax_t;\n#endif\n\n\n#cmakedefine uintptr_t @uintptr_t@\n\n\n#cmakedefine
  # HAVE_RESTRICT\n#cmakedefine HAVE___RESTRICT\n\n#cmakedefine
  # HAVE_INLINE\n#cmakedefine HAVE___INLINE\n\n#ifndef HAVE_RESTRICT\n#  ifdef
  # HAVE___RESTRICT\n# define LZMA_RESTRICT __restrict\n#  else\n#    define
  # LZMA_RESTRICT\n#  endif\n#else\n#  define LZMA_RESTRICT restrict\n#endif /*
  # HAVE_RESTRICT */\n\n#if defined(HAVE_INLINE)\n# define LZMA_INLINE
  # inline\n#else\n# if defined(HAVE___INLINE)\n#  define LZMA_INLINE
  # __inline\n# else\n#  define LZMA_INLINE\n# endif\n#endif\n\n#cmakedefine
  # WORDS_BIGENDIAN 1\n\n#cmakedefine HAVE_BYTESWAP_H 1\n#cmakedefine
  # HAVE_BSWAP_16 1\n#cmakedefine HAVE_BSWAP_32 1\n#cmakedefine HAVE_BSWAP_64
  # 1\n\n\n#define HAVE_CHECK_CRC32 1\n#define HAVE_CHECK_CRC64 1\n#define
  # HAVE_CHECK_SHA256 1\n\n#define HAVE_DECODER_ARM 1\n#define
  # HAVE_DECODER_ARMTHUMB 1\n#define HAVE_DECODER_DELTA 1\n#define
  # HAVE_DECODER_IA64 1\n#define HAVE_DECODER_LZMA1 1\n#define
  # HAVE_DECODER_LZMA2 1\n#define HAVE_DECODER_POWERPC 1\n#define
  # HAVE_DECODER_SPARC 1\n#define HAVE_DECODER_X86 1\n\n#define HAVE_ENCODER_ARM
  # 1\n#define HAVE_ENCODER_ARMTHUMB 1\n#define HAVE_ENCODER_DELTA 1\n#define
  # HAVE_ENCODER_IA64 1\n#define HAVE_ENCODER_LZMA1 1\n#define
  # HAVE_ENCODER_LZMA2 1\n#define HAVE_ENCODER_POWERPC 1\n#define
  # HAVE_ENCODER_SPARC 1\n#define HAVE_ENCODER_X86 1\n\n#define HAVE_MF_BT2
  # 1\n#define HAVE_MF_BT3 1\n#define HAVE_MF_BT4 1\n#define HAVE_MF_HC3
  # 1\n#define HAVE_MF_HC4 1\n\n/* Define to 1 if you have the <inttypes.h>
  # header file. */\n#cmakedefine HAVE_INTTYPES_H 1\n\n/* Define to 1 if you
  # have the <limits.h> header file. */\n#cmakedefine HAVE_LIMITS_H 1\n\n/*
  # Define to 1 if you have the <memory.h> header file. */\n#cmakedefine
  # HAVE_MEMORY_H 1\n\n/* Define to 1 if stdbool.h conforms to C99.
  # */\n#cmakedefine HAVE_STDBOOL_H 1\n\n/* Define to 1 if you have the
  # <stdint.h> header file. */\n#cmakedefine HAVE_STDINT_H 1\n\n/* Define to 1
  # if you have the <strings.h> header file. */\n#cmakedefine HAVE_STRINGS_H
  # 1\n\n/* Define to 1 if you have the <string.h> header file. */\n#cmakedefine
  # HAVE_STRING_H 1\n\n/* Define to 1 if you have the <sys/byteorder.h> header
  # file. */\n#cmakedefine HAVE_SYS_BYTEORDER_H 1\n\n/* Define to 1 if you have
  # the <sys/endian.h> header file. */\n#cmakedefine HAVE_SYS_ENDIAN_H 1\n\n/*
  # Define to 1 or 0, depending whether the compiler supports simple
  # visibility\n   declarations. */\n#cmakedefine HAVE_VISIBILITY 1\n\n/* Define
  # to 1 if the system has the type `_Bool'. */\n#cmakedefine HAVE__BOOL 1\n\n/*
  # Define to 1 if the system supports fast unaligned access to 16-bit and\n
  # 32-bit integers. */\n#if defined(__i386) || defined(__i386__) ||
  # defined(_M_IX86) \\n || defined(__x86_64) || defined(__x86_64__) ||
  # defined(_M_X64) \\n || defined(__amd64) || defined(__amd64__) \\n ||
  # defined(__powerpc) || defined(__powerpc__) \\n || defined(__ppc) ||
  # defined(__ppc__) || defined(__POWERPC__)\n# define
  # TUKLIB_FAST_UNALIGNED_ACCESS 1\n#endif")

  configure_file("${CMAKE_SOURCE_DIR}/3rdparty/lzma-config.h.cmake"
                 "${CMAKE_BINARY_DIR}/3rdparty/xz/config.h")

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
    set_target_properties(
      lzma PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xz"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/xz")
    set(LZMA_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/xz")
    set(LIBLZMA_INCLUDE_DIRS
        ${LZMA_SOURCE_DIR}/src/common
        ${LZMA_SOURCE_DIR}/src/liblzma/lzma
        ${LZMA_SOURCE_DIR}/src/liblzma/lz
        ${LZMA_SOURCE_DIR}/src/liblzma/check
        ${LZMA_SOURCE_DIR}/src/liblzma/simple
        ${LZMA_SOURCE_DIR}/src/liblzma/delta
        ${LZMA_SOURCE_DIR}/src/liblzma/rangecoder
        ${LZMA_SOURCE_DIR}/src/liblzma/common
        ${LZMA_SOURCE_DIR}/src/liblzma/api
        ${CMAKE_BINARY_DIR}/3rdparty/xz)
    define_have(
      LIBLZMA_DEFS
      HAVE_STDINT_H
      HAVE_STDLIB_H
      HAVE_STDBOOL_H
      HAVE_INTTYPES_H
      HAVE_LIMITS_H
      HAVE_STRING_H
      HAVE_STRINGS_H
      HAVE_MEMORY_H
      HAVE_SYS_PARAM_H
      HAVE_SYS_TIME_H
      HAVE_SYS_TYPES_H
      HAVE_LZMA_H)
    # message("LIBLZMA_DEFS: ${LIBLZMA_DEFS}")
    set_target_properties(
      lzma PROPERTIES COMPILE_DEFINITIONS "${LIBLZMA_DEFS}"
                      INCLUDE_DIRECTORIES "${LIBLZMA_INCLUDE_DIRS}")
  endif(LIBLZMA_SOURCES)

  link_directories(BEFORE "${CMAKE_BINARY_DIR}/xz")
  set(LIBLZMA_LIBRARY
      lzma
      CACHE FILEPATH "" FORCE)
  set(LIBLZMA_INCLUDE_DIR
      "${CMAKE_SOURCE_DIR}/3rdparty/xz/src/liblzma/api"
      CACHE FILEPATH "" FORCE)
endif(BUILD_LZMA)

find_program(TAR NAMES bsdtar tar gtar gnutar star)

if(BUILD_SSL)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
      file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
      file(DOWNLOAD "https://www.openssl.org/source/openssl-1.1.1f.tar.gz"
           "${CMAKE_SOURCE_DIR}/openssl.tar.gz" SHOW_PROGRESS)
    endif()

    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty" -xzf
                               "${CMAKE_SOURCE_DIR}/openssl.tar.gz")
    file(RENAME "${CMAKE_SOURCE_DIR}/3rdparty/openssl-1.1.1f"
         "${CMAKE_SOURCE_DIR}/3rdparty/openssl")
    exec_program("${TAR}" ARGS -C "${CMAKE_SOURCE_DIR}/3rdparty/openssl" -xzf
                               "${CMAKE_SOURCE_DIR}/openssl-cmake.tar.gz")

    # file(READ "${CMAKE_SOURCE_DIR}/build/cmake/BuildOpenSSL.cmake"
    # OPENSSL_CMAKELISTS) file(WRITE
    # "${CMAKE_SOURCE_DIR}/3rdparty/openssl/CMakeLists.txt"
    # "${OPENSSL_CMAKELISTS}")
    #
    file(
      WRITE "${CMAKE_BINARY_DIR}/openssl-pthread.patch"
      "diff -w -r -u -- crypto/threads_pthread.c crypto/threads_pthread.c\n--- crypto/threads_pthread.c\n2020-04-11 06:10:13.642470720 +0200\n+++ crypto/threads_pthread.c\n2020-04-11 06:10:21.212373541 +0200\n@@ -45,7 +45,7 @@\n     }\n \n     pthread_mutexattr_init(&attr);\n-    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);\n+    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);\n \n     if (pthread_mutex_init(lock, &attr) != 0) {\n         pthread_mutexattr_destroy(&attr);\n"
    )

    # file(WRITE "${CMAKE_BINARY_DIR}/openssl-poly1305_ieee754.patch" "---
    # 3rdparty/openssl/crypto/poly1305/poly1305_ieee754.corig     2020-02-13
    # 09:37:58.389826334 +0100\n+++
    # 3rdparty/openssl/crypto/poly1305/poly1305_ieee754.c    2020-02-13
    # 09:33:12.801691620 +0100\n@@ -104,7 +104,7 @@\n #elif defined(__mips__)\n
    # static const u32 fcsr = 1;\n #else\n-#error \"unrecognized
    # platform\"\n+#warning \"unrecognized platform\"\n #endif\n \n int
    # poly1305_init(void *ctx, const unsigned char key[16])")

    exec_program(patch "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/openssl"
                 ARGS -p0 -i "${CMAKE_BINARY_DIR}/openssl-pthread.patch")
  endif()

  add_definitions(-DOPENSSL_NO_UI_CONSOLE=1)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DOPENSSL_NO_UI_CONSOLE=1")
  add_subdirectory(3rdparty/openssl)

  set(OPENSSL_LIBRARIES
      ssl crypto
      CACHE STRING "" FORCE)
  set(OPENSSL_INCLUDE_DIR
      "${CMAKE_SOURCE_DIR}/3rdparty/openssl/include"
      CACHE FILEPATH "" FORCE)
endif()

if(USE_SSL)
  add_definitions(-DHAVE_OPENSSL=1 -DOPENSSL_API_COMPAT=0x10200000L)
  if(OPENSSL_INCLUDE_DIR)
    include_directories(${CMAKE_BINARY_DIR}/3rdparty/openssl/include
                        ${OPENSSL_INCLUDE_DIR})
  endif()

  # if(NOT OPENSSL_LIBRARIES) if(OPENSSL_CRYPTO_LIBRARY OR OPENSSL_SSL_LIBRARY)
  set(OPENSSL_LIBARRIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
  # endif() endif()
endif(USE_SSL)

link_directories(AFTER "${CMAKE_BINARY_DIR}")

file(TO_NATIVE_PATH "/" PATH_SEPARATOR)
message("The systems's path separator is '${PATH_SEPARATOR}'")

message("BZIP2_INCLUDE_DIR: ${BZIP2_INCLUDE_DIR}")
message("INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}")
message("LIBLZMA_INCLUDE_DIR: ${LIBLZMA_INCLUDE_DIR}")
message("LIBLZMA_INCLUDE_DIRS: ${LIBLZMA_INCLUDE_DIRS}")
message("OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}")
message("ZLIB_INCLUDE_DIR: ${ZLIB_INCLUDE_DIR}")

set(iarray_SOURCES lib/cas.S)

set(MODSUBDIRS
    alloc
    array
    binfmt
    buffer
    byte
    case
    cb
    cbmap
    charbuf
    coff
    dir
    dns
    elf
    env
    errmsg
    fmt
    gpio
    hashmap
    hmap
    http
    iarray
    ihex
    io
    json
    list
    map
    mmap
    ndelay
    omf
    open
    path
    pe
    playlist
    process
    range
    rdir
    safemult
    scan
    seek
    set
    sig
    slist
    socket
    str
    stralloc
    strarray
    strlist
    tai
    taia
    textbuf
    textcode
    uint16
    uint32
    uint64
    unix
    wait
    xml
    ucs
    utf8)

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
    set_target_properties(
      "${LIBNAME}-shared"
      PROPERTIES PREFIX "lib" COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS} -fPIC")
  endif(BUILD_SHARED_LIBS)
  set_target_properties(
    "${LIBNAME}"
    PROPERTIES PREFIX "" COMPILE_FLAGS
                         "${WARN_C_COMPILER_FLAGS} ${${LIBNAME}_COMPILE_FLAGS}")

endforeach(LIBNAME)

set_target_properties(io PROPERTIES COMPILE_FLAGS "${MUX_DEFS}")

foreach(NETLIB dns http io ndelay socket)
  target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()

target_link_libraries(unix stralloc buffer ndelay)
target_link_libraries(array safemult byte str)
target_link_libraries(binfmt mmap byte)
target_link_libraries(
  buffer
  alloc
  byte
  fmt
  mmap
  open
  str
  ${ZLIB_LIBRARY})
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(
  dns
  byte
  case
  fmt
  io
  open
  socket
  str
  taia
  uint16
  ${LIBIPHLPAPI})
target_link_libraries(dir stralloc)
target_link_libraries(errmsg str)
target_link_libraries(
  elf
  byte
  str
  range
  mmap
  uint64
  uint32
  uint16)
target_link_libraries(fmt byte str uint64 uint32)
target_link_libraries(gpio mmap)
target_link_libraries(hmap str)
target_link_libraries(
  http
  dns
  errmsg
  byte
  io
  scan
  socket
  stralloc
  str
  ${OPENSSL_LIBARRIES})
target_link_libraries(
  io
  buffer
  iarray
  array
  byte
  fmt
  open
  taia
  ${LIBMSWSOCK})
target_link_libraries(json charbuf hmap scan)
target_link_libraries(map str)
target_link_libraries(mmap seek open scan ${LIBPSAPI})
target_link_libraries(open str uint32)
target_link_libraries(path dir strlist)
target_link_libraries(pe uint64 uint32 uint16)
target_link_libraries(playlist byte fmt scan str xml)
target_link_libraries(rdir dir)
target_link_libraries(scan byte str)
target_link_libraries(
  socket
  byte
  fmt
  ndelay
  scan
  uint16
  ${LIBMSWSOCK})
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

set(genmakefile_SOURCES mplab.c ini.c ini.h genmakefile.h debug.c debug.h)

if(OPENSSL_LIB_DIR)
  set(CMAKE_EXE_LINKER_FLAGS "-L${OPENSSL_LIB_DIR}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  option(USE_EFENCE "Enable electric fence" OFF)
endif()

if(CMAKE_C_COMPILER MATCHES diet)
  set(DIET TRUE)
endif()

if(USE_EFENCE)
  if(DIET)
    set(ELECTRICFENCE_LIBRARY efence pthread)
  else(DIET)
    set(ELECTRICFENCE_LIBRARY efence)
  endif(DIET)
endif(USE_EFENCE)

set(COMMON_LIBS
    strlist
    array
    safemult
    scan
    str
    byte
    mmap
    open
    fmt)
file(GLOB MODHEADERS lib/*.h)

function(add_program PROG)
  unset(INCLUDES)
  unset(LIBS)
  foreach(DEP ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}"
       AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
      list(APPEND "${PROG}_SOURCES" "${DEP}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}"
         AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
      list(FIND MODSUBDIRS "${DEP}" I)
      if(I GREATER -1)
        list(APPEND INCLUDES "lib/${DEP}.h")
      endif(I GREATER -1)
      list(APPEND DEPS ${DEP})
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}"
          AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
  endforeach(DEP ${ARGN})
  # message("Program '${PROG}' dependencies: ${INCLUDES}")
  add_executable(${PROG} ${PROG}.c ${${PROG}_SOURCES} ${INCLUDES})
  target_link_libraries(${PROG} ${DEPS} ${ELECTRICFENCE_LIBRARY}
                        ${EXTRA_LIBRARIES})
  install(TARGETS ${PROG} DESTINATION bin)
  if(NOT EMSCRIPTEN
     AND OPENSSL_FOUND
     AND USE_SSL)
    set_target_properties(
      ${PROG}
      PROPERTIES
        LINK_FLAGS
        "-L${OPENSSL_LIB_DIR} -Wl,-rpath=${OPENSSL_LIB_DIR} ${EXE_LINKER_FLAGS}"
        COMPILE_FLAGS "")
  endif(
    NOT EMSCRIPTEN
    AND OPENSSL_FOUND
    AND USE_SSL)
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
      set_target_properties(${MOD} PROPERTIES EXE_LINKER_FLAGS
                                              "-L${OPENSSL_LIB_DIR} -shared")
    endif(BUILD_SHARED_LIBS)
    target_compile_options(${MOD} PUBLIC -fPIC)
  endif(NOT EMSCRIPTEN)
endfunction(add_module MOD)

if(NOT LIBLZMA_LIBRARY)
  set(LIBLZMA_LIBRARY "${LIBLZMA_LIBRARY}")
endif(NOT LIBLZMA_LIBRARY)

set(COMPRESSION_LIBRARIES ${LIBLZMA_LIBRARY} ${ZLIB_LIBRARY} ${BZIP2_LIBRARY})

if(OPENSSL_LIBRARIES)
  set(CRYPTO_LIBRARIES ${OPENSSL_LIBRARIES} pthread ${LIBCOMPAT})
endif(OPENSSL_LIBRARIES)

add_program(binfmttest binfmt)
if(USE_BZIP)
  add_program(
    bsdiffcat
    path
    errmsg
    buffer
    array
    safemult
    str
    byte
    errmsg
    uint32
    uint64
    ${COMPRESSION_LIBRARIES})
  add_program(ccat buffer open str unix ${COMPRESSION_LIBRARIES})
endif(USE_BZIP)
add_program(
  buffertest
  stralloc
  buffer
  array
  case
  mmap
  ${COMPRESSION_LIBRARIES})
add_program(
  compiler-wrapper
  stralloc
  buffer
  byte
  fmt
  str
  strlist
  dir)
add_program(count-depth buffer fmt)
add_program(
  decode-ls-lR
  stralloc
  buffer
  byte
  fmt
  io
  open
  str)
add_program(
  dnsip
  dns
  stralloc
  buffer
  errmsg
  socket
  ${LIBIPHLPAPI})
add_program(dnsname dns stralloc buffer errmsg ${LIBIPHLPAPI})
add_program(
  dnstest
  dns
  stralloc
  buffer
  errmsg
  ${LIBIPHLPAPI}
  ${LIBMSWSOCK})
add_program(
  eagle-gen-cmds
  xml
  stralloc
  buffer
  strarray
  array
  byte
  cb
  cbmap
  errmsg
  fmt
  hmap
  mmap
  scan
  str
  unix
  ${LIBMATH})
add_program(
  eagle-init-brd
  xml
  stralloc
  buffer
  hmap
  strlist
  scan
  str
  ${LIBMATH})
add_program(
  eagle-to-circuit
  xml
  stralloc
  buffer
  strlist
  array
  byte
  cb
  cbmap
  fmt
  hashmap
  hmap
  mmap
  scan
  str
  ${LIBMATH})
add_program(
  eagle-to-svg
  xml
  stralloc
  buffer
  strlist
  array
  byte
  cb
  cbmap
  fmt
  hashmap
  hmap
  mmap
  scan
  str)
add_program(elf64list elf mmap uint16 uint32 uint64)
add_program(
  elflist
  buffer
  alloc
  errmsg
  unix
  elf
  mmap
  str)
add_program(elfwrsec buffer alloc elf mmap)
add_program(
  genmakefile
  sig
  set
  case
  errmsg
  strarray
  slist
  rdir
  dir
  path
  strlist
  hashmap
  hmap
  stralloc
  buffer
  mmap
  unix
  open
  scan
  fmt
  byte
  str
  array
  safemult
  alloc)
add_program(
  piccfg
  ihex
  errmsg
  strarray
  slist
  rdir
  dir
  path
  strlist
  hashmap
  hmap
  stralloc
  buffer
  mmap
  unix
  open
  scan
  fmt
  byte
  str
  array
  safemult
  alloc)
add_program(
  hexedit
  stralloc
  buffer
  array
  io
  errmsg
  mmap
  open
  path
  scan
  uint64
  unix
  ${ZLIB_LIBRARY})
add_program(
  httptest
  http
  dns
  stralloc
  buffer
  errmsg
  byte
  io
  socket
  alloc
  taia
  ${LIBMSWSOCK}
  ${CRYPTO_LIBRARIES}
  ${LIBDL}
  pthread)
add_program(impgen pe buffer byte mmap open)
add_program(
  jsontest
  json
  stralloc
  buffer
  byte
  charbuf
  fmt
  hmap
  mmap
  open)
add_program(
  list-r
  path
  strarray
  strlist
  stralloc
  buffer
  array
  unix
  dir
  array
  fmt
  open
  str
  uint64
  ${LIBADVAPI32})
add_program(macho32list mmap uint32)
if(USE_LZMA)
  add_program(
    mediathek-list
    errmsg
    stralloc
    buffer
    alloc
    byte
    fmt
    http
    scan
    slist
    str
    unix
    strarray
    strlist
    ${LIBLZMA_LIBRARY}
    ${OPENSSL_LIBRARIES}
    ${CRYPTO_LIBRARIES}
    ${LIBDL}
    pthread)
endif(USE_LZMA)

add_program(
  mediathek-parser
  case
  ucs
  errmsg
  strarray
  strlist
  stralloc
  buffer
  array
  byte
  open
  str
  unix
  strlist)
add_program(
  ntldd
  stralloc
  buffer
  pe
  strlist
  byte
  path
  str
  unix
  uint64)
add_program(opensearch-dump xml stralloc buffer)
add_program(
  pathtool
  stralloc
  buffer
  errmsg
  path
  strlist
  unix)
add_program(
  pelist
  buffer
  errmsg
  mmap
  pe
  str
  unix)
add_program(
  omflist
  stralloc
  buffer
  errmsg
  omf
  mmap
  str
  unix)
add_program(
  pkgcfg
  wait
  path
  unix
  hashmap
  hmap
  stralloc
  buffer
  byte
  cbmap
  dir
  env
  errmsg
  slist
  case
  str
  strarray
  strlist)
add_program(
  plsconv
  stralloc
  buffer
  byte
  open
  playlist
  str
  unix)
add_program(rdir-test rdir stralloc buffer)
add_program(
  reg2cmd
  stralloc
  buffer
  byte
  fmt
  open
  scan
  str
  uint64)
add_program(
  regfilter
  stralloc
  buffer
  byte
  fmt
  open
  scan
  str
  uint64)
add_program(
  sln
  unix
  stralloc
  buffer
  byte
  errmsg
  path)
add_program(
  pipeinfo
  slist
  seek
  unix
  stralloc
  buffer
  array
  byte
  errmsg
  path)
add_program(
  strarraytest
  xml
  dns
  stralloc
  buffer
  mmap
  strarray
  unix)
add_program(
  torrent-progress
  stralloc
  buffer
  io
  mmap
  open
  uint64)
add_program(
  xmlpp
  errmsg
  xml
  stralloc
  buffer
  hmap
  io
  unix)
add_program(
  xmltest
  xml
  stralloc
  buffer
  byte
  fmt
  hmap)
add_program(
  xmltest2
  xml
  stralloc
  buffer
  byte
  fmt
  hmap)
add_program(
  xmltest3
  xml
  stralloc
  buffer
  byte
  fmt
  hmap)
add_program(
  xmltest4
  xml
  stralloc
  buffer
  array
  byte
  cb
  cbmap
  fmt
  hmap
  mmap
  scan
  str
  strlist)
# add_program(ziptest buffer str unix ${COMPRESSION_LIBRARIES})
add_program(
  cc-wrap
  errmsg
  env
  buffer
  path
  dir
  strarray
  strlist
  stralloc
  fmt)
add_program(
  ar-wrap
  path
  process
  errmsg
  env
  buffer
  dir
  strarray
  strlist
  stralloc
  fmt
  alloc)
add_program(
  cofflist
  coff
  unix
  errmsg
  stralloc
  buffer
  mmap)
add_program(
  msys-shell
  unix
  errmsg
  env
  buffer
  path
  strarray
  strlist
  stralloc)
set(tcping_SOURCES address.c)
add_program(
  tcping
  hmap
  unix
  dns
  case
  io
  array
  socket
  ndelay
  errmsg
  taia
  tai
  buffer
  stralloc
  mmap
  open
  fmt
  scan
  str
  byte
  uint16
  ${LIBMSWSOCK}
  ${LIBIPHLPAPI})
set(ftptest_SOURCES address.c ftplib.c)
add_program(
  ftptest
  hmap
  unix
  dns
  case
  io
  array
  socket
  ndelay
  errmsg
  taia
  tai
  stralloc
  buffer
  mmap
  open
  fmt
  scan
  str
  byte
  uint16
  ${LIBMSWSOCK}
  ${LIBIPHLPAPI})


set(nscache_SOURCES
    cache.c
    cache.h
    log.c
    log.h
    query.c
    query.h
    response.c
    response.h
    roots.c
    roots.h
    names.c
    names.h)
add_program(
  nscache
  dns
  path
  errmsg
  dir
  array
  stralloc
  alloc
  env
  byte
  uint16
  ${LIBMSWSOCK}
  ${LIBIPHLPAPI})
add_program(crc errmsg buffer mmap open ${ZLIB_LIBRARY})
add_program(
  cmake-run
  errmsg
  env
  buffer
  path
  dir
  strarray
  strlist
  stralloc
  fmt)

add_program(jsonpp json errmsg unix)
add_program(xml2json json xml errmsg unix)
add_program(xml2moon xml textcode errmsg)
add_program(
  strip-comments
  errmsg
  env
  charbuf
  buffer
  path
  dir
  strarray
  strlist
  stralloc
  fmt)

add_program(httpproxy http io map)
add_program(
  testini
  ini.c
  ini.h
  hmap
  stralloc
  buffer
  mmap
  open
  uint16)
# add_program(montty io map)
add_program(
  logserial
  sig
  charbuf
  hmap
  path
  unix
  rdir
  dir
  errmsg
  io
  ndelay
  strarray
  stralloc
  buffer
  taia
  tai)
add_program(
  tinyproxy
  process
  unix
  path
  wait
  range
  dns
  taia
  tai
  hmap
  env
  ndelay
  buffer
  strarray
  array
  strlist
  stralloc
  errmsg
  sig
  socket
  fmt
  scan
  io
  uint32)
target_link_libraries(tinyproxy ${LIBMSWSOCK})
if(NOT WIN32)
  add_program(tcpproxy http io errmsg buffer open)
endif(NOT WIN32)
# add_program(redir http io ${LIBCOMPAT})

add_program(parse strlist errmsg)
add_program(testihex ihex stralloc buffer alloc mmap)
add_program(
  setuidgid
  errmsg
  path
  stralloc
  mmap
  open
  scan
  str)

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
