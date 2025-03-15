#include "../http.h"
#include "../alloc.h"

http_response*
http_response_new(http* h) {
  http_response* r;

  if((r = alloc_zero(sizeof(http_response)))) {
    stralloc_init(&r->data);
    stralloc_init(&r->boundary);

    r->content_length = (uint64)-1;
    r->status = 0;
    r->code = -1;
    r->next = h->response;
  }

  h->response = r;

  return r;
}
