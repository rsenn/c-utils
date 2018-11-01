#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCKAPI_
#include <winsock2.h>
#endif

#include "../socket_internal.h"

#include <sys/types.h>
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
