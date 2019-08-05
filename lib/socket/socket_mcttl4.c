#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

#if WINDOWS_NATIVE && !defined(IP_MULTICAST_TTL)
#include <ws2tcpip.h>
#endif

int
socket_mcttl4(int s, char TTL) {
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&TTL, sizeof TTL));
}

