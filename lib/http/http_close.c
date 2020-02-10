#include "../http.h"

void
http_close(http* h) {
  if(h->sock != -1)
    close(h->sock);
  h->sock = -1;
}
