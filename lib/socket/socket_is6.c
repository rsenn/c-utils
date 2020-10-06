#include "../socket_internal.h"

int
socket_is6(int s) {
#ifdef LIBC_HAS_IP6
  struct sockaddr_in6 si;
#else
  struct sockaddr_in si;
#endif
  socklen_t len = sizeof si;

  if(getsockname(s, (void*)&si, &len) == -1)
    return winsock2errno(-1);

#ifdef LIBC_HAS_IP6
  if(noipv6 || si.sin6_family == AF_INET || si.sin6_family == AF_INET)
    return 0;

  return si.sin6_family == AF_INET6;
#endif

  return 0;
}
