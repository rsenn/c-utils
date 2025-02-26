#include "../http.h"
#include "../alloc.h"

http_response*
http_response_new(void) {
  http_response* r;

  if((r = alloc_zero(sizeof(http_response)))) {
    stralloc_init(&r->data);
    stralloc_init(&r->boundary);
    r->content_length = (uint64)-1;
    r->status = HTTP_RECV_HEADER;
    r->code = -1;
  }

  return r;
}
