#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

ssize_t
socket_recv4(int s, char* buf, size_t len, char ip[4], uint16* port) {
  struct sockaddr_in si;
  socklen_t Len = sizeof si;
  ssize_t r;

  if((r = recvfrom(s, buf, len, 0, (struct sockaddr*)&si, &Len)) < 0)
    return winsock2errno(-1);

  if(ip)
    *(uint32*)ip = *(uint32*)&si.sin_addr;

  if(port)
    uint16_unpack_big((char*)&si.sin_port, port);
  return r;
}
