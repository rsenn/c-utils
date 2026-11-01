#include "../http.h"

/* appends data to a body of known length, 0 on failure */
int
http_body(http* h, const char* x, size_t n) {
  http_request* req = h->request;

  if(req == NULL || req->chunked)
    return 0;

  req->body_done = 1;
  return stralloc_catb(&req->body, x, n);
}
