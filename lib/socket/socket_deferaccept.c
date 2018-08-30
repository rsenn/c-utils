#include "../socket.h"

#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#endif
#include "../socket.h"
#include "../windoze.h"
#include <string.h>

void
socket_deferaccept(int s, enum defermode mode) {
#ifdef TCP_DEFER_ACCEPT
  int one = 1;
  (void)mode;
  setsockopt(s, IPPROTO_TCP, TCP_DEFER_ACCEPT, &one, sizeof(one));
#elif defined(SO_ACCEPTFILTER)
  struct accept_filter_arg afa;
  memset(&afa, 0, sizeof(afa));
  strcpy(afa.af_name, mode == HTTPIN ? "httpreader" : "dataready");
  setsockopt(s, SOL_SOCKET, SO_ACCEPTFILTER, &afa, sizeof(afa));
#endif
}
