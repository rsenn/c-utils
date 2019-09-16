#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../byte.h"

#include <sys/types.h>
#include <string.h>

void
socket_deferaccept(int s, enum defermode mode) {
#ifdef TCP_DEFER_ACCEPT
  int one = 1;
  (void)mode;
  setsockopt(s, IPPROTO_TCP, TCP_DEFER_ACCEPT, (void*)&one, sizeof(one));
#elif defined(SO_ACCEPTFILTER)
  struct accept_filter_arg afa;
  byte_zero(&afa, sizeof(afa));
  str_copy(afa.af_name, mode == HTTPIN ? "httpreader" : "dataready");
  setsockopt(s, SOL_SOCKET, SO_ACCEPTFILTER, (void*)&afa, sizeof(afa));
#endif
}
