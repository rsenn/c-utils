#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#define USE_WS2_32 1
#include "../windoze.h"

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../ip4.h"
#include "../byte.h"

// #include <netinet/in.h>

int
socket_mcjoin4(int s, const char ip[4], const char _interface[4]) {
#ifdef IP_ADD_MEMBERSHIP
  struct ip_mreq opt;
  byte_copy(&opt.imr_multiaddr.s_addr, 4, ip);
  byte_copy(&opt.imr_interface.s_addr, 4, _interface);
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&opt, sizeof opt));
#else
  return -1;
#endif
}
