#if defined(__MINGW32__) || defined(__BORLANDC__)
#include "../io_internal.h"
#include <errno.h>
int64
io_receivefd(fd_type sock) {
  errno = EINVAL;
  return -1;
}
#else

#ifdef __NetBSD__
#define _NETBSD_SOURCE
#endif

#ifdef __OpenBSD__
#warning This is a HORRIBLE kludge around yet another thing OpenBSD broke
#warning If they make it possible to do file descriptor passing with
#warning _XOPEN_SOURCE defined, please send me an email so I can remove this.
#warning _XOPEN_SOURCE needs to be defined for this to work on Solaris.
#else
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1
#endif
#include <stddef.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <errno.h>
#include <string.h>
#include "../io_internal.h"

union fdmsg {
  struct cmsghdr h;
  /* on NetBSD, CMSG_SPACE is not constant */
  char buf[1000];
};

/* receives the descriptor io_passfd sent over the unix socket sock, returns
 * it, or -1 with errno set (EPROTO when the byte came without a descriptor) */
int64
io_receivefd(fd_type sock) {
  struct msghdr msg;
  struct iovec iov;
#ifdef CMSG_FIRSTHDR
  union fdmsg cmsg;
  struct cmsghdr* h;
#else
  int fd;
#endif
  char x;
  char name[100];

  memset(&msg, 0, sizeof(msg));
  iov.iov_base = &x;
  iov.iov_len = 1;
  msg.msg_name = name;
  msg.msg_namelen = sizeof(name);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
#ifdef CMSG_FIRSTHDR
  memset(&cmsg, 0, sizeof(cmsg));
  msg.msg_control = cmsg.buf;
  msg.msg_controllen = sizeof(union fdmsg);
#else
  msg.msg_accrights = (char*)&fd;
  msg.msg_accrightslen = sizeof(fd);
#endif

  if(recvmsg(sock, &msg, 0) == -1)
    return -1;

#ifdef CMSG_FIRSTHDR
  h = CMSG_FIRSTHDR(&msg);
#ifndef CMSG_LEN
#define CMSG_LEN(x) x
#endif

  if(!h || h->cmsg_len != CMSG_LEN(sizeof(int)) || h->cmsg_level != SOL_SOCKET || h->cmsg_type != SCM_RIGHTS) {
#ifdef EPROTO
    errno = EPROTO;
#else
    errno = EINVAL;
#endif
    return -1;
  }

  return *((int*)CMSG_DATA(h));
#else
  if(msg.msg_accrightslen != sizeof(fd))
    return -1;

  return fd;
#endif
}
#endif
