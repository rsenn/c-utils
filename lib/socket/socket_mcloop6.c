#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

int
socket_mcloop6(int s, char loop) {
#ifdef IP_MULTICAST_LOOP
#ifdef LIBC_HAS_IP6
  if(noipv6)
    return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&loop, sizeof loop));
  else
    return winsock2errno(setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, (void*)&loop, sizeof loop));
#else
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&loop, sizeof loop));
#endif

#else
  return -1;
#endif
}
