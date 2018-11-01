#include "../windoze.h"
#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCK2API_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include "../byte.h"
#include "../uint16.h"
#include "../uint32.h"

#if !WINDOWS_NATIVE
#include <netinet/in.h>
#endif

int
socket_connect4(int s, const char *ip, uint16 port) {
  struct sockaddr_in si;
  byte_zero(&si, sizeof(si));
  si.sin_family = AF_INET;
  uint16_pack_big((char*) &si.sin_port, port);
  *((uint32*)&si.sin_addr) = *((uint32*)ip);
  return winsock2errno(connect(s, (struct sockaddr*)&si, sizeof(si)));
}
