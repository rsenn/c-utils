#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>
#include "../byte.h"

ssize_t
socket_send4(int s, const char* buf, size_t len, const char ip[4], uint16 port) {
  return socket_send4_flag(s, buf, len, ip, port, 0);
}
