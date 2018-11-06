#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

int
socket_broadcast(int s) {
  int opt = 1;
  return winsock2errno(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &opt, sizeof opt));
}
