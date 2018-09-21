#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* defined(HAVE_CONFIG_H) */

#include <sys/types.h>
#if !WINDOWS_NATIVE
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
