#include "../windoze.h"
#define USE_WS2_32 1

#if WINDOWS
//#define _WINSOCK2API_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include "../ndelay.h"

#include <sys/types.h>

int socket_tcp4b(void) {
  int s;
  __winsock_init();
  s = winsock2errno(socket(PF_INET,SOCK_STREAM,IPPROTO_TCP));
  if(s == -1) return -1;
  return s;
}
