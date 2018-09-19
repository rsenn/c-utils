#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* defined(HAVE_CONFIG_H) */

#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../ndelay.h"
#include "../socket.h"
#include "../windoze.h"
#include <errno.h>

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT EINVAL
#endif
#ifndef EPFNOSUPPORT
#define EPFNOSUPPORT EAFNOSUPPORT
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT EAFNOSUPPORT
#endif

int
socket_sctp6b(void) {
#ifndef IPPROTO_SCTP
  errno = EPROTONOSUPPORT;
  return -1;
#else
#ifdef LIBC_HAS_IP6
  int s;

  __winsock_init();
  if(noipv6) goto compat;
  s = winsock2errno(socket(PF_INET6, SOCK_STREAM, IPPROTO_SCTP));
  if(s == -1) {
    if(errno == EINVAL || errno == EAFNOSUPPORT || errno == EPFNOSUPPORT || errno == EPROTONOSUPPORT) {
    compat:
      s = winsock2errno(socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP));
      noipv6 = 1;
      if(s == -1) return -1;
    } else
      return -1;
  }
#ifdef IPV6_V6ONLY
  {
    int zero = 0;
    winsock2errno(setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&zero, sizeof(zero)));
  }
#endif
  return s;
#else
  return socket_sctp4b();
#endif
#endif
}
