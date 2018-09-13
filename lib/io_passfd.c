#include "windoze.h"
#ifdef _WIN32
#include <winsock.h>
#define cmsghdr wsacmsghdr
#endif

#if WINDOWS
#include "io_internal.h"
#include <errno.h>
int
io_passfd(fd_t sock, fd_t fd) {
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
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1
#endif
#include "io_internal.h"
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

union fdmsg {
  struct cmsghdr h;
  /* on NetBSD, CMSG_SPACE is not constant */
  /*  char buf[CMSG_SPACE(sizeof(int))]; */
  char buf[1000];
};

int
io_passfd(fd_t sock, fd_t fd) {
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
  iov.iov_base = "x";
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
