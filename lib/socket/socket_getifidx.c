#define USE_WS2_32 1
#include "../socket.h"
#include <sys/types.h>

uint32
socket_getifidx(const char* ifname) {
#ifdef HAVE_N2I
  return if_nametoindex(ifname);
#else
  return 0;
#endif
}
