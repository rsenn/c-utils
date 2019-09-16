#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <errno.h>

ssize_t
socket_fastopen_connect6(int s, const char* ip, uint16 port, uint32 scope_id, const char* buf, size_t len) {
#ifndef MSG_FASTOPEN
  int r;
  {
#else
  int r = socket_send6_flag(s, buf, len, ip, port, scope_id, MSG_FASTOPEN);
  if(r == -1 && errno == ENOTCONN) {
#endif
    /* apparently the kernel does not support TCP fast open */
    r = socket_connect6(s, ip, port, scope_id);
    if(r == 0)
      return write(s, buf, len);
  }
  return r;
}
