#include <sys/param.h>
#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../byte.h"
#include "../socket.h"
#include "../windoze.h"

int
socket_remote4(int s, char ip[4], uint16* port) {
  struct sockaddr_in si;
  socklen_t len = sizeof si;

  if(getpeername(s, (struct sockaddr*)&si, &len) == -1) return winsock2errno(-1);
  if(ip) *(uint32*)ip = *(uint32*)&si.sin_addr;
  if(port) uint16_unpack_big((char*)&si.sin_port, port);
  return 0;
}
