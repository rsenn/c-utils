AC_INIT([dirlist],[20180919])

AC_CONFIG_SRCDIR([list-r.c])
AC_CONFIG_AUX_DIR([config])

dnl canonicalize the host and os so we can do some tricky things before
dnl parsing options
AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AM_DEP_TRACK
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB
AC_PROG_INSTALL

AC_LANG([C])
AC_EXEEXT

AC_HEADER_STDC

AC_CHECK_HEADERS([alloca.h errno.h inttypes.h linux/limits.h stdbool.h stdint.h sys/fcntl.h])

AC_HEADER_ASSERT
AC_HEADER_DIRENT
AC_HEADER_STAT
AC_HEADER_TIME


AC_FUNC_ALLOCA
AC_FUNC_FNMATCH
AC_FUNC_LSTAT
dnl AC_FUNC_MMAP
dnl AC_FUNC_STRERROR_R

AC_C_INLINE
AC_C_INLINE

AC_TYPE_OFF_T
AC_TYPE_OFF_T

AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UNSIGNED_LONG_LONG_INT

AC_DIAGNOSE([obsolete],[your code should no longer depend upon `LONG_64_BITS', but upon
`SIZEOF_LONG_INT == 8'.  Remove this warning and the `AC_DEFINE' when
you adjust the code.])dnl
AC_CHECK_SIZEOF([long int])
test $ac_cv_sizeof_long_int = 8 &&
  AC_DEFINE([LONG_64_BITS],[1],[Define to 1 if `sizeof (long int)' = 8.  Obsolete, use
	     `SIZEOF_LONG_INT'.])


AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_INTMAX_T
AC_TYPE_UINTMAX_T
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

dnl AC_CHECK_FUNC([posix_memalign],[AC_DEFINE_UNQUOTED([HAVE_POSIX_MEMALIGN],[1],[Define this if you have the posix_memalign() function])])
dnl AC_CHECK_FUNC([aligned_alloc],[AC_DEFINE_UNQUOTED([HAVE_ALIGNED_ALLOC],[1],[Define this if you have the aligned_alloc() function])])
dnl AC_CHECK_FUNC([_aligned_malloc],[AC_DEFINE_UNQUOTED([HAVE__ALIGNED_MALLOC],[1],[Define this if you have the _aligned_malloc() function])])
dnl AC_CHECK_FUNC([alloca],[AC_DEFINE_UNQUOTED([HAVE_ALLOCA],[1],[Define this if you have the alloca() function])])
AC_CHECK_FUNC([fnmatch],[AC_DEFINE_UNQUOTED([HAVE_FNMATCH],[1],[Define this if you have the fnmatch() function])])
AC_CHECK_FUNC([inet_pton],[AC_DEFINE_UNQUOTED([HAVE_INET_PTON],[1],[Define this if you have the inet_pton() function])])
AC_CHECK_FUNC([llseek],[AC_DEFINE_UNQUOTED([HAVE_LLSEEK],[1],[Define this if you have the llseek() function])])
AC_CHECK_FUNC([lseek],[AC_DEFINE_UNQUOTED([HAVE_LSEEK],[1],[Define this if you have the lseek() function])])
AC_CHECK_FUNC([lseek64],[AC_DEFINE_UNQUOTED([HAVE_LSEEK64],[1],[Define this if you have the lseek64() function])])
AC_CHECK_FUNC([lstat],[AC_DEFINE_UNQUOTED([HAVE_LSTAT],[1],[Define this if you have the lstat() function])])
AC_CHECK_FUNC([pipe2],[AC_DEFINE_UNQUOTED([HAVE_PIPE2],[1],[Define this if you have the pipe2() function])])
AC_CHECK_FUNC([getdelim],[AC_DEFINE_UNQUOTED([HAVE_GETDELIM],[1],[Define this if you have the getdelim() function])])
AC_CHECK_FUNC([round],[AC_DEFINE_UNQUOTED([HAVE_ROUND],[1],[Define this if you have the round() function])])
AC_CHECK_FUNC([wordexp],[AC_DEFINE_UNQUOTED([HAVE_WORDEXP],[1],[Define this if you have the wordexp() function])])
dnl AC_CHECK_FUNC([WSAStartup],[AC_DEFINE_UNQUOTED([HAVE_WSASTARTUP],[1],[Define this if you have the WSAStartup() function])])

AC_CHECK_ZLIB
AC_CHECK_BZLIB
AC_CHECK_LIBLZMA

dnl Check for configuration options:
# --enable-debug/--disable-debug
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [build with debugging support [default=no]])],
  [], [enable_debug=no])
  
AS_IF([test "x$enable_debug" != "xno"],
  [AC_DEFINE([DEBUG], [1], [Enable debugging support.]) 
CFLAGS=`echo "$CFLAGS" | sed 's|-O2|-O0|g ;; s|-g[0-9]*|-g3 -ggdb3|g'
],
  [AC_DEFINE([NDEBUG], [1], [Disable assertions.])])
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$enable_debug" != "xno"])

# --enable-widechar/--disable-widechar
AC_ARG_ENABLE([widechar],
  [AS_HELP_STRING([--enable-widechar], [include support for wide char])])
AS_IF([test "x$enable_widechar" == "xyes"],
  [AC_DEFINE_UNQUOTED([USE_WIDECHAR], 1, [Define to enable wide car support.])])

# --enable-readdir/--disable-readdir
AC_ARG_ENABLE([readdir],
  [AS_HELP_STRING([--enable-readdir], [include support for readdir])])
  
  
AS_IF([test "x$enable_readdir" = "xyes" -o "x$enable_readdir" != "xno"],
  [
  AC_CHECK_FUNC([readdir],[
    AC_DEFINE_UNQUOTED([USE_READDIR], 1, [Define to enable readdir support.])
  ])
])

dnl
dnl  OS feature checks
dnl 

dnl ---------------------------------------------------------------------------
dnl Checking for IPv6
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for IPv6 support])
AC_COMPILE_IFELSE([#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main() {
  struct sockaddr_in6 sa;
  sa.sin6_family = PF_INET6;
  (void)sa;
  return 0;
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([LIBC_HAS_IP6],[1],[Define this if you have IPv6 support])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for scope id
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for IP scope id support])
AC_COMPILE_IFELSE([#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main() {
  struct sockaddr_in6 sa;
  sa.sin6_family = PF_INET6;
  sa.sin6_scope_id = 23;
  (void)sa;
  return 0;
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([LIBC_HAS_SCOPE_ID],[1],[Define this if you have scope id support])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for if_indextoname
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for if_indextoname])
AC_COMPILE_IFELSE([#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
int main() {
  static char ifname[IFNAMSIZ];
  char *tmp=if_indextoname(0,ifname);
  (void)tmp;
  return 0;
}],[AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_N2I],[1],[Define this if you have if_indextoname])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for kqueue
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for kqueue support])
AC_COMPILE_IFELSE([#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
int
main() {
  int kq = kqueue();
  struct kevent kev;
  struct timespec ts;
  if(kq == -1) return 111;
  EV_SET(&kev, 0 /* fd */, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
  ts.tv_sec = 0;
  ts.tv_nsec = 0;
  if(kevent(kq, &kev, 1, 0, 0, &ts) == -1) return 111;
  {
    struct kevent events[100];
    int i, n;
    ts.tv_sec = 1;
    ts.tv_nsec = 0;
    switch(n = kevent(kq, 0, 0, events, 100, &ts)) {
    case -1: return 111;
    case 0: puts("no data on fd #0"); break;
    }
    for(i = 0; i < n; ++i) {
      printf("ident %d, filter %d, flags %d, fflags %d, data %d\n",
             events[i].ident,
             events[i].filter,
             events[i].flags,
             events[i].fflags,
             events[i].data);
    }
  }
  return 0;
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_KQUEUE],[1],[Define this if you have kqueue support])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for BSD sendfile
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for BSD compatible sendfile])
AC_COMPILE_IFELSE([#define SENDFILE 1
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
int
main() {
  struct sf_hdtr hdr;
  struct iovec v[17 + 23];
  int r, fd = 1;
  off_t sbytes;
  hdr.headers = v;
  hdr.hdr_cnt = 17;
  hdr.trailers = v + 17;
  hdr.trl_cnt = 23;
  r = sendfile(0, 1, 37, 42, &hdr, &sbytes, 0);
  (void)r;
  return 0;
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_BSDSENDFILE],[1],[Define this if you have a BSD compatible sendfile()])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for sendfile
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for sendfile])
AC_COMPILE_IFELSE([#ifdef __hpux__
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
int
main() {
  /* sbsize_t sendfile(int s, int fd, off_t offset, bsize_t nbytes,
                       const struct iovec *hdtrl, int flags); */
  struct iovec x[2];
  int fd = open("havesendfile.c", 0);
  x[0].iov_base = "header";
  x[0].iov_len = 6;
  x[1].iov_base = "footer";
  x[1].iov_len = 6;
  sendfile(1 /* dest socket */, fd /* src file */, 0 /* offset */, 23 /* nbytes */, x, 0);
  perror("sendfile");
  return 0;
}
#elif defined(__sun__) && defined(__svr4__)
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
int
main() {
  off_t o;
  o = 0;
  sendfile(1 /* dest */, 0 /* src */, &o, 23 /* nbytes */);
  perror("sendfile");
  return 0;
}
#elif defined(_AIX)
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int
main() {
  int fd = open("havesendfile.c", 0);
  struct sf_parms p;
  int destfd = 1;
  p.header_data = "header";
  p.header_length = 6;
  p.file_descriptor = fd;
  p.file_offset = 0;
  p.file_bytes = 23;
  p.trailer_data = "footer";
  p.trailer_length = 6;
  if(send_file(&destfd, &p, 0) >= 0) printf("sent %lu bytes.\n", p.bytes_sent);
  return 0;
}
#elif defined(__linux__)
#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <unistd.h>
#if defined(__GLIBC__)
#include <sys/sendfile.h>
#elif defined(__dietlibc__)
#include <sys/sendfile.h>
#else
#include <linux/unistd.h>
_syscall4(int, sendfile, int, out, int, in, long*, offset, unsigned long, count)
#endif
#include <stdio.h>
int
main() {
  int fd = open("havesendfile.c", 0);
  off_t o = 0;
  off_t r = sendfile(1, fd, &o, 23);
  if(r != -1) printf("sent %llu bytes.\n", r);
  return 0;
}
#else
#error unsupported architecture
/* stupid fucking IRIX c99 does not signal #error via a non-zero exit
 * code!  ARGH!  So insert a parse error */
#include rumpelstilzchen
)
#endif], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_SENDFILE],[1],[Define if you have sendfile()])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for epoll
dnl ---------------------------------------------------------------------------
AC_DEFUN([AC_CHECK_EPOLL], [
AC_MSG_CHECKING(m4_ifval([$2],[for epoll using $2],[for epoll]))
m4_ifval([$2],[saved_LIBS="$LIBS"
LIBS="$2"],[])
AC_LINK_IFELSE([#include <inttypes.h>
#include <stdio.h>
#include <sys/epoll.h>
int
main() {
  int efd = epoll_create(10);
  struct epoll_event x;
  if(efd == -1) return 111;
  x.events = EPOLLIN;
  x.data.fd = 0;
  if(epoll_ctl(efd, EPOLL_CTL_ADD, 0 /* fd */, &x) == -1) return 111;
  {
    int n;
    struct epoll_event y[100];
    if((n = epoll_wait(efd, y, 100, 1000)) == -1) return 111;
    if(n > 0) printf("event %d on fd #%d\n", y[0].events, y[0].data.fd);
  }
  return 0;
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_EPOLL],[m4_ifval([$1],[$1],[1])],[Define if you have epoll()])
$3
],[AC_MSG_RESULT([no])
$4])
m4_ifval([$2],[LIBS="$saved_LIBS"],[])
])

AC_CHECK_EPOLL([1],[],[],[AC_CHECK_EPOLL([2],[-lepoll])])

dnl ---------------------------------------------------------------------------
dnl Checking for devpoll
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for devpoll])
AC_COMPILE_IFELSE([#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/devpoll.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
main() {
  int fd = open("/dev/poll", O_RDWR);
  struct pollfd p[100];
  int i, r;
  dvpoll_t timeout;
  p[0].fd = 0;
  p[0].events = POLLIN;
  write(fd, p, sizeof(struct pollfd));
  timeout.dp_timeout = 100; /* milliseconds? */
  timeout.dp_nfds = 1;
  timeout.dp_fds = p;
  r = ioctl(fd, DP_POLL, &timeout);
  for(i = 0; i < r; ++i) printf("event %d on fd #%d\n", p[i].revents, p[i].fd);
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_DEVPOLL],[1],[Define if you have devpoll])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for sigio
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for SIGIO support])
AC_COMPILE_IFELSE([#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/types.h>
int
main() {
  int signum = SIGRTMIN + 1;
  sigset_t ss;
  sigemptyset(&ss);
  sigaddset(&ss, signum);
  sigaddset(&ss, SIGIO);
  sigprocmask(SIG_BLOCK, &ss, 0);
  fcntl(0 /* fd */, F_SETOWN, getpid());
  fcntl(0 /* fd */, F_SETSIG, signum);
#if defined(O_ONESIGFD) && defined(F_SETAUXFL)
  fcntl(0 /* fd */, F_SETAUXFL, O_ONESIGFD);
#endif
  fcntl(0 /* fd */, F_SETFL, fcntl(0 /* fd */, F_GETFL) | O_NONBLOCK | O_ASYNC);
  {
    siginfo_t info;
    struct timespec timeout;
    int r;
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;
    switch((r = sigtimedwait(&ss, &info, &timeout))) {
    case SIGIO:
      /* signal queue overflow */
      signal(signum, SIG_DFL);
      /* do poll */
      break;
    default:
      if(r == signum) {
        printf("event %c%c on fd #%d\n",
               info.si_band & POLLIN ? 'r' : '-',
               info.si_band & POLLOUT ? 'w' : '-',
               info.si_fd);
      }
    }
  }
  return 0;
}], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_SIGIO],[1],[Define this if you have sigio support])],
[AC_MSG_RESULT([no])])

dnl ---------------------------------------------------------------------------
dnl Checking for __uint128_t
dnl ---------------------------------------------------------------------------
AC_MSG_CHECKING([for __uint128_t type])
AC_COMPILE_IFELSE([__uint128_t tmp;
], [AC_MSG_RESULT([yes])
AC_DEFINE_UNQUOTED([HAVE_UINT128],[1],[Define this if you have the __uint128_t type])],
[AC_MSG_RESULT([no])])


AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

