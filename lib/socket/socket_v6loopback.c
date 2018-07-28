#include "../socket.h"

const unsigned char
V6loopback[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };

const unsigned char*
socket_ip6loopback() { return V6loopback; }