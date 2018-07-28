#include "../socket.h"

const char ip4loopback[4] = {127,0,0,1};

const unsigned char* socket_ip4loopback() {
  return ip4loopback;
}
