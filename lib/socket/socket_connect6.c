#include "../byte.h"
#include "../windoze.h"
#if WINDOWS_NATIVE 
#include <winsock.h>
#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include "../ip6.h"
#include "../socket.h"
#include "../uint16.h"
#include "../uint32.h"
#include <errno.h>

int socket_connect6(int s, const char ip[16], uint16 port, uint32 scope_id) {
#ifdef LIBC_HAS_IP6
  struct sockaddr_in6 sa;

  if(noipv6) {
#endif
    if(ip6_isv4mapped(ip))
      return winsock2errno(socket_connect4(s, ip + 12, port));
    if(byte_equal(ip, 16, socket_ip6loopback()))
      return winsock2errno(socket_connect4(s, socket_ip4loopback(), port));
#ifdef LIBC_HAS_IP6
  }
  byte_zero(&sa, sizeof sa);
  sa.sin6_family = PF_INET6;
  uint16_pack_big((char *) &sa.sin6_port, port);
  sa.sin6_flowinfo = 0;
#ifdef LIBC_HAS_SCOPE_ID
  sa.sin6_scope_id = scope_id;
#endif
  byte_copy((char *) &sa.sin6_addr, 16, ip);

  return winsock2errno(connect(s, (void*) &sa, sizeof sa));
#else
    errno = EPROTONOSUPPORT;
    return -1;
#endif
}
