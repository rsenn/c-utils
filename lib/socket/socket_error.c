#define USE_WS2_32 1
#define __wasilibc_unmodified_upstream 1

#include "../socket_internal.h"

int
socket_error(fd_t s, int* error) {
  socklen_t errlen = sizeof(int);
  return getsockopt(s, SOL_SOCKET, SO_ERROR, (void*)error, &errlen) == 0;
}
