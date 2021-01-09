#define _GNU_SOURCE
#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../byte.h"

#include <sys/types.h>
//#include <netinet/in.h>

int
socket_mcleave4(int s, const char ip[4]) {
#ifdef IP_DROP_MEMBERSHIP
  struct ip_mreq opt;
  byte_copy(&opt.imr_multiaddr.s_addr, 4, ip);
  byte_zero(&opt.imr_interface.s_addr, 4);
  return winsock2errno(
      setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&opt, sizeof opt));
#else
  return -1;
#endif
}
