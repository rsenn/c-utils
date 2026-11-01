#include "../http.h"
#include "../io.h"

/* queues the last chunk */
int
http_chunk_end(http* h) {
  http_request* req = h->request;

  if(!http_chunked(h))
    return 0;

  if(req->body_done)
    return 1;

  if(!stralloc_catb(&req->body, "0\r\n\r\n", 5))
    return 0;

  req->body_done = 1;

  if(h->sent && h->sock != -1)
    io_wantwrite(h->sock);

  return 1;
}
