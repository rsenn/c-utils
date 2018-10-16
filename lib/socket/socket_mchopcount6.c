#define USE_WS2_32 1
#include "../socket.h"
#include "../ip6.h"

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
