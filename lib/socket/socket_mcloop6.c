#include "../windoze.h"
#include "../socket_internal.h"
#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
/*#include "haveip6.h"*/

#ifndef IP_MULTICAST_LOOP
#define IP_MULTICAST_LOOP 11 /* winsock2.h; missing in the winsock.h of Borland C++ */
#endif

int
socket_mcloop6(int s, char loop) {
#ifdef LIBC_HAS_IP6
  if(noipv6)
    return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof loop));
  else
    return winsock2errno(setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loop, sizeof loop));
#else
  return winsock2errno(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof loop));
#endif
}
