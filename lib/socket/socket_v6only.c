#include "../socket_internal.h"

int
socket_v6only(int fd, int enable) {
  int prevstate = -1;
#ifdef IPV6_V6ONLY
  socklen_t optlen = sizeof(prevstate);

  if(getsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&prevstate, &optlen) == -1)
    return -1;

  if(setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&enable, sizeof(enable)) == -1)
    return -1;
#endif
  return prevstate;
}