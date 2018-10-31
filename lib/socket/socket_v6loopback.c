#include "../windoze.h"
#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCK2API_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
const unsigned char
V6loopback[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };

const char*
socket_ip6loopback() { return (const char*)V6loopback; }
