#include "../windoze.h"
#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCK2API_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
const unsigned char ip4loopback[4] = {127,0,0,1};

const char*
socket_ip4loopback() {
  return (const char*)ip4loopback;
}
