#define USE_WS2_32 1
#include "../socket.h"

#include <sys/types.h>
#include "../socket_internal.h"
#include "../ndelay.h"

int socket_tcp4b(void) {
  int s;
  __winsock_init();
  s = winsock2errno(socket(PF_INET,SOCK_STREAM,IPPROTO_TCP));
  if(s == -1) return -1;
  return s;
}
