set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_WARN_VS8 OFF)

set(CMAKE_C_STANDARD 11)

project(c-utils C ASM)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Functions.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Macros.cmake)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation prefix" FORCE)
endif()
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

set(CONFIG_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Installation prefix" FORCE)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type")

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release MinSizeRel RelWithDebInfo)

option(USE_LZMA "lzma compression" ON)
option(USE_BZIP "bzip2 compression" ON)
option(USE_ZLIB "zlib compression" ON)
option(USE_BROTLI "brotli compression" ON)
option(USE_SSL "ssl encryption" ON)

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

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  set(CLANG TRUE CACHE BOOL "Clang compiler type" FORCE)
endif(CMAKE_C_COMPILER_ID STREQUAL "Clang")

#[[if(CLANG)
  set(CMAKE_C_CLANG_TIDY clang-tidy -checks=-*,readability-*)
  message(STATUS "Clang tidy checks: ${CMAKE_C_CLANG_TIDY}")
endif(CLANG)
]]
#[[option(USE_POLL "Use poll() syscall" OFF)
option(USE_EPOLL "Use epoll() syscall" OFF)]]
option(BUILD_SHARED_LIBS "Build shared libs" "${SHARED_LIBS}")
option(LINK_STATIC "Link libraries static" "${STATIC_LINK}")
option(ENABLE_WERROR "Enable -Werror (warnings are errors)" OFF)

debug_option(IO "Debug I/O code")
debug_option(HTTP "Debug HTTP code")
debug_option(TLS "Debug TLS code")
debug_option(OUTPUT "Debug output")
debug_option(CPP "Debug C preprocessor")
debug_option(TOKENIZER "Debug C tokenizer")
debug_option(CHARBUF "Debug character buf")
debug_option(JSON "Debug JSON")

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

foreach(C_STANDARD c2x c18 c17 c1x c11 c99)
  string(REPLACE "c" "CPLUSPLUS" C_STANDARD_NUM "${C_STANDARD}")
  set(CMAKE_REQUIRED_QUIET TRUE)
  check_c_compiler_flag("-std=${C_STANDARD}" C_STANDARD_${C_STANDARD_NUM})
  set(CMAKE_REQUIRED_QUIET FALSE)
  if(C_STANDARD_${C_STANDARD_NUM})
    set(C_STANDARD_VALUE "${C_STANDARD}" CACHE STRING "C standard" FORCE)
    set(C_STANDARD_FLAG "-std=${C_STANDARD}" CACHE STRING "C standard argument" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_STANDARD_FLAG}")
    break()
  endif(C_STANDARD_${C_STANDARD_NUM})
endforeach()

message(STATUS "C standard: ${C_STANDARD_VALUE}")

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

set(TRY_LINUX_AIO
    "#include <fcntl.h>\n#include <linux/aio_abi.h>\n#include <stdint.h>\n#include <stdio.h>\n#include <stdlib.h>\n#include <sys/syscall.h>\n#include <unistd.h>\n\n#define PFATAL(x...) \\\\\n  do { \\\\\n    fprintf(stderr, \"[-] SYSTEM ERROR : \" x); \\\\\n    fprintf(stderr, \"\\\\n\\tLocation : %s(), %s:%u\\\\n\", __FUNCTION__, __FILE__, __LINE__); \\\\\n    perror(\"      OS message \"); \\\\\n    fprintf(stderr, \"\\\\n\"); \\\\\n    exit(EXIT_FAILURE); \\\\\n  } while(0)\n\n#ifdef __dietlibc__\n#include <bits/syscall.h>\n#else\ninline static int\nio_setup(unsigned nr, aio_context_t* ctxp) {\n  return syscall(__NR_io_setup, nr, ctxp);\n}\n\ninline static int\nio_destroy(aio_context_t ctx) {\n  return syscall(__NR_io_destroy, ctx);\n}\n\ninline static int\nio_submit(aio_context_t ctx, long nr, struct iocb** iocbpp) {\n  return syscall(__NR_io_submit, ctx, nr, iocbpp);\n}\n\ninline static int\nio_getevents(aio_context_t ctx,\n             long min_nr,\n             long max_nr,\n             struct io_event* events,\n             struct timespec* timeout) {\n  // This might be improved.\n  return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);\n}\n#endif\n\n#define BUF_SZ (4096)\n\nint\nmain() {\n  int fd = open(\"/etc/passwd\", O_RDONLY);\n  if(fd < 0) {\n    PFATAL(\"open(/etc/passwd)\");\n  }\n\n  aio_context_t ctx = 0;\n  int r = io_setup(128, &ctx);\n  if(r < 0) {\n    PFATAL(\"io_setup()\");\n  }\n\n  char buf[BUF_SZ];\n  struct iocb cb = {.aio_fildes = fd,\n                    .aio_lio_opcode = IOCB_CMD_PREAD,\n                    .aio_buf = (uint64_t)&buf[0],\n                    .aio_nbytes = BUF_SZ};\n  struct iocb* list_of_iocb[1] = {&cb};\n\n  r = io_submit(ctx, 1, list_of_iocb);\n  if(r != 1) {\n    PFATAL(\"io_submit()\");\n  }\n\n  struct io_event events[1] = {};\n  r = io_getevents(ctx, 1, 1, events, NULL);\n  if(r != 1) {\n    PFATAL(\"io_getevents()\");\n  }\n\n  printf(\"read %ld bytes from /etc/passwd\\\\n\", events[0].res);\n  io_destroy(ctx);\n  close(fd);\n  return 0;\n}\n"
)
check_compile(HAVE_LINUX_AIO "${TRY_LINUX_AIO}")
if(HAVE_LINUX_AIO)
  check_run(SUPPORT_LINUX_AIO "${TRY_LINUX_AIO}")
endif(HAVE_LINUX_AIO)

if(HAVE_LINUX_AIO)
  message(STATUS "Support Linux AIO: ${SUPPORT_LINUX_AIO}")
  add_definitions(-DHAVE_LINUX_AIO=1)
endif(HAVE_LINUX_AIO)

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

  check_compile(HAVE_SIGSET_T "#define _GNU_SOURCE\n#include <sys/types.h>\n#include <signal.h>\n#include <fcntl.h>\n#include <stdio.h>\n\nint main() {\n  sigset_t ss;\nprintf(\"%lu\\\\n\", sizeof(ss));\n  return 0;\n}\n" SIZEOF_SIGSET_T)
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

string(REGEX REPLACE ".*/" "" COMPILER_NAME "${CMAKE_C_COMPILER}")

if(COMPILER_NAME MATCHES "em.*")
  set(EMSCRIPTEN TRUE)
  # dump(EMSCRIPTEN EMCC_SUFFIX EMSCRIPTEN_PREFIX EMSCRIPTEN_TOOLCHAIN_PATH COMPILER_NAME CMAKE_EXE_LINKER_FLAGS) foreach(LIB BZIP ZLIB LZMA SSL) set("USE_${LIB}" OFF) endforeach(LIB BZIP ZLIB LZMA SSL)
  set(EMSCRIPTEN_EXE_SUFFIX "html")
endif(COMPILER_NAME MATCHES "em.*")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  check_c_compiler_flag("-O0" O_OPT_NONE)
  if(O_OPT_NONE)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
  endif(O_OPT_NONE)
  check_c_compiler_flag("-ggdb" G_OPT_GDB)
  if(G_OPT_GDB)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
  endif(G_OPT_GDB)
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
  set(EXE_LINKER_FLAGS "-Wl,-rpath=${OPENSSL_LIB_DIR}")
  set(CHK_DLFCN TRUE)
endif(LINK_STATIC)

set(CMAKE_STATIC_LINKER_FLAGS "")

check_functions(lstat waitpid aligned_alloc fnmatch getdelim getopt getopt_long pipe2 popen posix_memalign pread sendfile sigaddset sigemptyset sigprocmask strerror_r __write writev posix_spawnattr_setflags posix_spawn_file_actions_init posix_spawn_file_actions_adddup2 posix_spawnp clock_gettime)

check_include_file(linux/aio_abi.h HAVE_LINUX_AIO_ABI_H)

if(HAVE_LINUX_AIO_ABI_H)
  message(STATUS "Have Linux AIO ABI header")
  # list(APPEND CMAKE_REQUIRED_INCLUDES  "linux/aio_abi.h" )
  check_symbol_exists(io_setup "linux/aio_abi.h" HAVE_IO_SETUP)
  check_symbol_exists(io_destroy "linux/aio_abi.h" HAVE_IO_DESTROY)
  check_symbol_exists(io_submit "linux/aio_abi.h" HAVE_IO_SUBMIT)
  check_symbol_exists(io_getevents "linux/aio_abi.h" HAVE_IO_GETEVENTS)

  #[[foreach(H HAVE_IO_SETUP HAVE_IO_DESTROY HAVE_IO_SUBMIT HAVE_IO_GETEVENTS)
    add_definitions(-D"${H}=1")
  endforeach()]]
endif(HAVE_LINUX_AIO_ABI_H)

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

set(IOPAUSE_MUX "" CACHE STRING "iopause() multiplex call")

if(HAVE_LINUX_AIO)
  set(IOPAUSE_MUX LINUX_AIO CACHE STRING "iopause() multiplex call" FORCE)
elseif(SUPPORT_POLL)
  set(IOPAUSE_MUX POLL CACHE STRING "iopause() multiplex call" FORCE)
else()
  set(IOPAUSE_MUX SELECT CACHE STRING "iopause() multiplex call" FORCE)
endif()
add_definitions(-DIOPAUSE_${IOPAUSE_MUX}=1)

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

check_library_exists(pthread pthread_create "" HAVE_PTHREAD_LIB)
if(HAVE_PTHREAD_LIB)
  set(LIBPTHREAD pthread)
endif()
message(STATUS "LIBPTHREAD = ${LIBPTHREAD}")

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/FindStandardMathLibrary.cmake)

# check_library_exists(m log2 "" HAVE_MATH_LIB) if(HAVE_MATH_LIB) set(LIBMATH m CACHE STRING "Math library" FORCE) endif()
message(STATUS "LIBMATH = ${LIBMATH}")

if(NOT LINK_STATIC)
  check_library_exists(dl dlopen "" HAVE_DLFCN_LIB)
  if(HAVE_DLFCN_LIB)
    set(LIBDL dl)
  endif()
endif(NOT LINK_STATIC)

# include(${CMAKE_SOURCE_DIR}/FindZlib.cmake) include(${CMAKE_SOURCE_DIR}/FindBZip2.cmake) include(${CMAKE_SOURCE_DIR}/FindLibLZMA.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/CheckInline.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Profile.cmake)

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

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/Werror.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/CheckBrotli.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/CheckBZip2.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/CheckLZMA.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/CheckOpenSSL.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/CheckZlib.cmake)

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
    LINUX_AIO KQUEUE DEVPOLL EPOLL
    # SIGIO
    POLL SELECT)
set(MUX_USE LINUX_AIO KQUEUE EPOLL DEVPOLL SIGIO POLL SELECT)
foreach(METHOD ${MUX_USE})
  if(SUPPORT_${METHOD})
    list_contains("${MUX_LIST}" "${METHOD}" CONTAINS_METHOD)
    if(NOT CONTAINS_METHOD)
      list(APPEND MUX_LIST ${METHOD})
    endif(NOT CONTAINS_METHOD)
    if(MUX STREQUAL "")
      message(STATUS "Setting MUX method to ${METHOD}")
      set(MUX "${METHOD}" CACHE STRING "I/O multiplex method" FORCE)
    endif(MUX STREQUAL "")
    # set(MUX_DEFS "${MUX_DEFS} -DHAVE_${METHOD}=1")
  endif(SUPPORT_${METHOD})
endforeach()
#[[foreach(METHOD ${MUX_ALL})
  if(SUPPORT_${METHOD})
    set(MUX_DEFS "${MUX_DEFS} -DSUPPORT_${METHOD}=1")
  endif(SUPPORT_${METHOD})
endforeach()]]
set(MUX_METHODS "${MUX_LIST}")

message(STATUS "Available MUX methods: ${MUX_METHODS}")
string(REGEX REPLACE "[;\n ]" ";" __MUX_METHODS "${MUX_METHODS}")
# unset(MUX_METHODS)
set(MUX_METHOD "${MUX}" CACHE STRING "multiplex methods")

set_property(CACHE MUX_METHOD PROPERTY STRINGS ${__MUX_METHODS})

# message("MUX definitions: ${MUX_DEFS}")

if(LIBMSWSOCK)
  list(APPEND CMAKE_REQUIRED_LIBRARIES ${LIBMSWSOCK})
endif(LIBMSWSOCK)

message(STATUS "MUX method: ${MUX_METHOD}")

add_definitions("-DHAVE_${MUX_METHOD}=1" "-DUSE_${MUX_METHOD}=1")
set(io_COMPILE_FLAGS "-DMUX_METHOD=${MUX_METHOD} -DHAVE_${MUX_METHOD}=1 -DUSE_${MUX_METHOD}=1")

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

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/BuildBrotli.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/BuildBZip2.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/BuildLZMA.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/BuildSSL.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/BuildZlib.cmake)

# unset(TLS_LIBRARIES) unset(TLS_LIBRARIES CACHE) set(TLS_LIBRARIES "")
#
# if(OPENSSL_FOUND OR BUILD_OPENSSL) if(OPENSSL_LIBRARIES) set(TLS_LIBRARIES "${OPENSSL_LIBRARIES}" CACHE PATH "OpenSSL library") else(OPENSSL_LIBRARIES) if(LINK_STATIC) set(TLS_LIBRARIES "${OPENSSL_STATIC_LIBRARIES}" CACHE PATH "OpenSSL library") endif(LINK_STATIC) endif(OPENSSL_LIBRARIES)
#
# if(OPENSSL_LIB_DIR) set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${OPENSSL_LIB_DIR}") endif() if(OPENSSL_LIBRARIES) if(LINK_STATIC) set(CRYPTO_LIBRARIES ${OPENSSL_STATIC_LIBRARIES} pthread ${LIBCOMPAT}) else(LINK_STATIC) set(CRYPTO_LIBRARIES ${OPENSSL_LIBRARIES} pthread ${LIBCOMPAT}) endif(LINK_STATIC) endif(OPENSSL_LIBRARIES)
#
# else(OPENSSL_FOUND OR BUILD_OPENSSL) set(TLS_LIBRARIES "" CACHE PATH "OpenSSL library" FORCE) endif(OPENSSL_FOUND OR BUILD_OPENSSL)

dump(TLS_LIBRARIES)

link_directories(AFTER "${CMAKE_BINARY_DIR}")

file(TO_NATIVE_PATH "/" PATH_SEPARATOR)
# message("The systems's path separator is '${PATH_SEPARATOR}'")

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

if(NOT LIBLZMA_LIBRARY)
  set(LIBLZMA_LIBRARY "${LIBLZMA_LIBRARY}")
endif(NOT LIBLZMA_LIBRARY)

set(COMPRESSION_LIBRARIES ${LIBLZMA_LIBRARY} ${ZLIB_LIBRARY} ${BZIP2_LIBRARY} ${BROTLI_LIBRARY} ${LIBMATH} CACHE STRING "Compression libraries" FORCE)
# dump(COMPRESSION_LIBRARIES)

if(LINK_STATIC)
  message(STATUS "Static linking")
  if(NOT CMAKE_EXE_LINKER_FLAGS MATCHES "-static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static" CACHE STRING "Executable linker flags" FORCE)
  endif(NOT CMAKE_EXE_LINKER_FLAGS MATCHES "-static")
endif(LINK_STATIC)

add_definitions(-DHAVE_CONFIG_H=1)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

# ------------------------------------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------------------------------------
# ##############################################################################################################################################################################################################################################################################################################################################################################################################
# #### END OF CHECKS, NOW COME THE TARGETS #### ####
# ##############################################################################################################################################################################################################################################################################################################################################################################################################

# dump(CMAKE_EXE_LINKER_FLAGS TLS_LIBRARIES)

add_subdirectory(lib ${CMAKE_BINARY_DIR}/lib)

set(COMMON_LIBS strlist array safemult scan str byte mmap open fmt)

set(genmakefile_SOURCES mplab.c ini.c ini.h genmakefile.h debug.c debug.h)

add_program(binfmttest binfmt)
if(USE_BZIP)
  add_program(bsdiffcat path errmsg buffer array safemult str byte errmsg uint32 uint64 ${COMPRESSION_LIBRARIES})
  add_program(ccat path case buffer open str unix ${COMPRESSION_LIBRARIES} ${LIBMATH})
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
set(httptest_SOURCES uri.c uri.h)

message("TLS_LIBRARIES = ${TLS_LIBRARIES}")
add_program(httptest slist xml sig http c dns stralloc buffer errmsg byte io socket alloc taia ${TLS_LIBRARIES} ${LIBMSWSOCK} pthread)
# add_program(ssltest strlist unix dns stralloc buffer errmsg byte io  tls socket alloc taia ${TLS_LIBRARIES} ${LIBMSWSOCK} ${LIBDL} pthread)

# set(ssltest_SOURCES address.c address.h)

add_program(impgen pe buffer byte mmap open)
add_program(jsontest json stralloc charbuf buffer byte fmt hmap mmap open)
set(list_r_SOURCES dump.c dump.h)
add_program(list-r errmsg path strarray strlist stralloc buffer array unix dir array fmt open str uint64 ${LIBADVAPI32})
add_program(macho32list mmap uint32)
if(USE_LZMA)
  add_program(mediathek-list errmsg stralloc buffer alloc byte fmt http tls scan slist str unix strarray strlist ${LIBLZMA_LIBRARY} ${TLS_LIBRARIES} ${LIBDL} pthread)
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
add_program(xmlscantest xml errmsg)
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

add_program(httpproxy http io hmap)
add_program(testini ini.c ini.h hmap stralloc buffer mmap open uint16)
# add_program(montty io hmap)
add_program(logserial sig charbuf hmap path unix rdir dir errmsg io ndelay strarray stralloc buffer taia tai)
add_program(tinyproxy process unix path wait range dns taia tai hmap env ndelay buffer strarray array strlist stralloc errmsg sig socket fmt scan io uint32)
target_link_libraries(tinyproxy ${LIBMSWSOCK})
# if(NOT WIN32) add_program(tcpproxy http io errmsg buffer open ) endif(NOT WIN32)

# add_program(redir http io ${LIBCOMPAT})

add_program(parse strlist errmsg)
add_program(testihex ihex stralloc buffer alloc mmap)
add_program(setuidgid errmsg path stralloc mmap open scan str)
add_program(lsproc strlist errmsg mmap buffer unix dir)
add_program(cpptest cpp tokenizer unix buffer open)
add_program(tokenizertest errmsg tokenizer unix buffer open)

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
