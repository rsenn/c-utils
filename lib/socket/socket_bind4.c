#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
#include "../byte.h"
#include "../socket.h"
#include "../uint16.h"
#include "../uint32.h"
#include "../windoze.h"

int
socket_bind4(int s, const char* ip, uint16 port) {
  struct sockaddr_in si;
  byte_zero(&si, sizeof si);
  si.sin_family = AF_INET;
  uint16_pack_big((char*)&si.sin_port, port);
  if(ip)
    *(uint32*)&si.sin_addr = *(uint32*)ip;
  else
    si.sin_addr.s_addr = INADDR_ANY;
  return winsock2errno(bind(s, (struct sockaddr*)&si, sizeof si));
}