#define _WINSOCKAPI_
#include "../socket.h"

#include <sys/types.h>
#if !(defined(_WIN32) || defined(_WIN64))
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "../socket_internal.h"
#include "../ndelay.h"

int socket_tcp4b(void) {
  int s;
  __winsock_init();
  s = winsock2errno(socket(PF_INET,SOCK_STREAM,IPPROTO_TCP));
  if(s == -1) return -1;
  return s;
}
