#include "../socket.h"

const unsigned char
V6loopback[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };

const char*
socket_ip6loopback() { return (const char*)V6loopback; }
