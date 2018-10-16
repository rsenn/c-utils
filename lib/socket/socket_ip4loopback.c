#define USE_WS2_32 1
#include "../socket.h"
const unsigned char ip4loopback[4] = {127,0,0,1};

const char*
socket_ip4loopback() {
  return (const char*)ip4loopback;
}
