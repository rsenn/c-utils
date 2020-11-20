#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../ndelay.h"

#include <errno.h>

int
socket_tcp4b(void) {
  int s;
  __winsock_init();
  s = winsock2errno(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
  if(s == -1)
    return -1;
  return s;
}
