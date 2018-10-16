#define _WINSOCKAPI_
#include "../socket.h"
#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#if WINDOWS && !defined(IP_MULTICAST_TTL)
#include <ws2tcpip.h>
#endif

int
socket_mcttl4(int s, char TTL) {
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, &TTL, sizeof TTL));
}
