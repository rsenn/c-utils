#include <sys/param.h>
#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../byte.h"
#include "../ip6.h"
#include "../socket.h"
#include "../windoze.h"

#ifndef IPV6_MULTICAST_HOPS
#undef LIBC_HAS_IP6
#endif

int
socket_mchopcount6(int s, char TTL) {
#ifdef LIBC_HAS_IP6
  return winsock2errno(setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &TTL, sizeof TTL));
#else
  return winsock2errno(socket_mcttl4(s, TTL));
#endif
}
