#include "../http.h"
#include "../alloc.h"

http_request*
http_request_new(http* h) {
  http_request* req;

  if((req = alloc_zero(sizeof(http_request)))) {
    stralloc_init(&req->location);
    stralloc_init(&req->headers);

    req->type = GET;
    req->serial = 0;
    req->next = h->request;
  }

  h->request = req;

  return req;
}
