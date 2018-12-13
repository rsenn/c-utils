#include "../socket_internal.h"

int
socket_error(fd_t s, int* error) {
  socklen_t errlen = sizeof(int);
  return getsockopt(s, SOL_SOCKET, SO_ERROR, error, &errlen) == 0;
}
