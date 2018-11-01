#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include "../byte.h"


int
socket_local4(int s, char ip[4], uint16* port) {
  struct sockaddr_in si;
  socklen_t len = sizeof si;

  if(getsockname(s, (struct sockaddr*)&si, &len) == -1) return winsock2errno(-1);
  if(ip) *(uint32*)ip = *(uint32*)&si.sin_addr;
  if(port) uint16_unpack_big((char*)&si.sin_port, port);
  return 0;
}
