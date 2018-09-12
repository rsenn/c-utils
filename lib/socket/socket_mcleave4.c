#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "../byte.h"
#include "../socket.h"
#include "../windoze.h"

int
socket_mcleave4(int s, const char ip[4]) {
  struct ip_mreq opt;
  byte_copy(&opt.imr_multiaddr.s_addr, 4, ip);
  byte_zero(&opt.imr_interface.s_addr, 4);
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &opt, sizeof opt));
}
