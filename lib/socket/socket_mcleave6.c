#include "../windoze.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* defined(HAVE_CONFIG_H) */

#include "../socket.h"
#include <errno.h>
#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#else
#ifdef IPV6_LEAVE_GROUP
#define LIBC_HAS_IP6
#endif
#endif
#include "../byte.h"
#include "../ip6.h"

#ifndef IPV6_DROP_MEMBERSHIP
#ifdef IPV6_LEAVE_GROUP
#define IPV6_DROP_MEMBERSHIP IPV6_LEAVE_GROUP
#else
#undef LIBC_HAS_IP6
#endif
#endif

int
socket_mcleave6(int s, const char ip[16]) {
#ifdef LIBC_HAS_IP6
  struct ipv6_mreq opt;
#endif
  if(ip6_isv4mapped(ip)) return socket_mcleave4(s, ip + 12);
#ifdef LIBC_HAS_IP6
  byte_copy(&opt.ipv6mr_multiaddr, 16, ip);
  opt.ipv6mr_interface = 0;
  return winsock2errno(setsockopt(s, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &opt, sizeof opt));
#else
  errno = EPROTONOSUPPORT;
  return -1;
#endif
}
