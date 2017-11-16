#include "http.h"

void
http_init(http* h, const char* host, uint16 port) {
  h->sock = -1;
  stralloc_init(&h->host);
  stralloc_init(&h->addr);
  stralloc_copys(&h->host, host);
  h->port = port;
  h->request = NULL;
}
