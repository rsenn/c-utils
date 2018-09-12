#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../windoze.h"

#include "../ndelay.h"
#include "../socket.h"

int
socket_udp4b(void) {
  int s;
  __winsock_init();
  s = winsock2errno(socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP));
  return s;
}

int
socket_udp4(void) {
  int s;
  s = socket_udp4b();
  if(s != -1 && ndelay_on(s) == -1) {
    close(s);
    return -1;
  }
  return s;
}
