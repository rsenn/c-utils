#include "../io_internal.h"
#include "../windoze.h"
#include "../socket_internal.h"
#ifdef _WIN32
#define cmsghdr wsacmsghdr
#endif

#if WINDOWS
#include <errno.h>
int
io_passfd(fd_type sock, fd_type fd) {
  errno = EINVAL;
  return -1;
}
#else

#ifdef __NetBSD__
#define _NETBSD_SOURCE
#endif

#ifdef __OpenBSD__
#pragma warning This is a HORRIBLE kludge around yet another thing OpenBSD broke
#pragma warning If they make it possible to do file descriptor passing with
#pragma warning _XOPEN_SOURCE defined, please send me an email so I can remove this.
#pragma warning _XOPEN_SOURCE needs to be defined for this to work on Solaris.
#else
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif
#endif
#include "../typedefs.h"
#include "../byte.h"
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>

#ifdef __wasi__
#define CMSG_FIRSTHDR
#define SCM_RIGHTS 0x01 /* rw: access rights (array of int) */

struct cmsghdr {
  size_t cmsg_len;    /* data byte count, including hdr */
  int32_t cmsg_level; /* originating protocol */
  int32_t cmsg_type;  /* protocol-specific type */
};
#endif

union fdmsg {
  struct cmsghdr h;
  /* on NetBSD, CMSG_SPACE is not constant */
  /*  char buf[CMSG_SPACE(sizeof(int))]; */
  char buf[1000];
};

int
io_passfd(fd_type sock, fd_type fd) {
  struct msghdr msg;
  struct iovec iov;
#ifdef CMSG_FIRSTHDR
  struct cmsghdr* cmsg;
#ifndef CMSG_SPACE
#define CMSG_SPACE(x) x + 100
#endif
  char buf[CMSG_SPACE(sizeof(int))];
  byte_zero(buf, sizeof(buf));
#endif
  byte_zero(&msg, sizeof(msg));
  iov.iov_len = 1;
  iov.iov_base = (void*)"x";
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_name = 0;
  msg.msg_namelen = 0;
#ifdef CMSG_FIRSTHDR
  msg.msg_control = buf;
  msg.msg_controllen = sizeof(buf);
  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
#ifndef CMSG_LEN
#define CMSG_LEN(x) x
#endif
  cmsg->cmsg_len = CMSG_LEN(sizeof(int));
  msg.msg_controllen = cmsg->cmsg_len;
  *((int*)CMSG_DATA(cmsg)) = fd;
#else
  msg.msg_accrights = (char*)&fd;
  msg.msg_accrightslen = sizeof(fd);
#endif
  return sendmsg(sock, &msg, 0) >= 0 ? 0 : -1;
}
#endif
