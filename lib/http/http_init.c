#define USE_WS2_32 1
#include "../socket_internal.h"
#define _WINSOCKAPI_
#include "../http.h"
#include "../byte.h"

void
http_init(http* h, const char* host, uint16 port) {
  byte_zero(h, sizeof(http));
  h->sock = -1;
  stralloc_init(&h->host);
  stralloc_copys(&h->host, host);
  h->port = port;
  h->request = NULL;
}
