#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../ndelay.h"

#include <errno.h>

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT EINVAL
#endif

#ifndef EPFNOSUPPORT
#define EPFNOSUPPORT EAFNOSUPPORT
#endif

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT EAFNOSUPPORT
#endif

int
socket_tcp6b(void) {
#ifdef LIBC_HAS_IP6
  int s;

#if WINDOWS_NATIVE
  __winsock_init();
#endif

  if(noipv6)
    goto compat;
  s = winsock2errno(socket(AF_INET6, SOCK_STREAM, 0));
  if(s == -1) {
    if(errno == EINVAL || errno == EAFNOSUPPORT || errno == EPFNOSUPPORT ||
       errno == EPROTONOSUPPORT) {
    compat:
      s = winsock2errno(socket(AF_INET, SOCK_STREAM, 0));
      noipv6 = 1;
      if(s == -1)
        return -1;
    } else
      return -1;
  }
#ifdef IPV6_V6ONLY
  {
    int zero = 0;
    winsock2errno(setsockopt(
        s, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&zero, sizeof(zero)));
  }
#endif
  return s;
#else
  return socket_tcp4b();
#endif
}
