#define _WINSOCKAPI_
#include "../socket.h"

#include <sys/types.h>
#if !(defined(_WIN32) || defined(_WIN64))
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "../socket_internal.h"
#include "../ndelay.h"
#include <errno.h>

int socket_tcp4(void) {
  int s;
#ifdef SOCK_NONBLOCK
  if((s = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP)) > -1 || errno != EINVAL) return s;
#endif
  s = socket_tcp4b();
  if(s == -1) return -1;
  if(ndelay_on(s) == -1) { closesocket(s); return -1; }
  return s;
}
