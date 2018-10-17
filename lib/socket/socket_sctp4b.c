#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>
#include "../ndelay.h"
#include <errno.h>

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT EINVAL
#endif

int
socket_sctp4b(void) {
#ifdef IPPROTO_SCTP
  int s;
  __winsock_init();
  s = winsock2errno(socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP));
  if(s == -1) return -1;
  return s;
#else
  errno = EPROTONOSUPPORT;
  return -1;
#endif
}
