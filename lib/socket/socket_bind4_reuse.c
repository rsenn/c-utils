#include "../windoze.h"
#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCK2API_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include <sys/types.h>

int
socket_bind4_reuse(int s, const char* ip, uint16 port) {
  int one = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
#ifdef SO_REUSEPORT
  setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &one, sizeof one);
#endif
  return winsock2errno(socket_bind4(s, ip, port));
}
