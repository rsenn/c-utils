#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <net/if.h>
#include <sys/socket.h>
#endif
#include "../socket.h"

uint32
socket_getifidx(const char* ifname) {
#ifdef HAVE_N2I
  return if_nametoindex(ifname);
#else
  return 0;
#endif
}
