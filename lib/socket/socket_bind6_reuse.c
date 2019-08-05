#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

int
socket_bind6_reuse(int s, const char* ip, uint16 port, uint32 scope_id) {
  int one = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void*)&one, sizeof one);
#ifdef SO_REUSEPORT
  setsockopt(s, SOL_SOCKET, SO_REUSEPORT, (void*)&one, sizeof one);
#endif
  return winsock2errno(socket_bind6(s, ip, port, scope_id));
}

