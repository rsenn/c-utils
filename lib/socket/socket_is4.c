#include "../socket_internal.h"

int
socket_is4(int s) {
#ifdef LIBC_HAS_IP6
  struct sockaddr_in6 si;
#else
  struct sockaddr_in si;
#endif
  socklen_t len = sizeof si;

  if(getsockname(s, (void*)&si, &len) == -1)
    return winsock2errno(-1);

#ifdef LIBC_HAS_IP6
  if(noipv6)
    return 1;

  return si.sin6_family == AF_INET;
#endif
  return 1;
}
