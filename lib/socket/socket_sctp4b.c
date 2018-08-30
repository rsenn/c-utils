#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include "../ndelay.h"
#include "../socket.h"
#include "../windoze.h"
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
