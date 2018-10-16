#define _WINSOCKAPI_
#include "../socket.h"
#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <netinet/in.h>
#include <sys/socket.h>
#endif

int
socket_broadcast(int s) {
  int opt = 1;
  return winsock2errno(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &opt, sizeof opt));
}
