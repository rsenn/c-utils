#include "../http.h"

/* declares that the body is sent as chunks, only possible before the request is sent */
int
http_chunked(http* h) {
  http_request* req = h->request;

  if(req == NULL || (!req->chunked && (req->queued || req->body.len)))
    return 0;

  req->chunked = 1;
  return 1;
}
