#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
const unsigned char ip4loopback[4] = {127, 0, 0, 1};

const char*
socket_ip4loopback() {
  return (const char*)ip4loopback;
}

