#include "../http.h"
#include "../fmt.h"
#include "../io.h"

/* queues x as one chunk of the body, 0 on failure. An empty chunk is not
 * sent, it would end the body, see http_chunk_end. */
int
http_chunk(http* h, const char* x, size_t n) {
  http_request* req = h->request;
  char num[FMT_XLONG];

  if(!http_chunked(h) || req->body_done)
    return 0;

  if(n == 0)
    return 1;

  if(!stralloc_catb(&req->body, num, fmt_xlong(num, n)) || !stralloc_catb(&req->body, "\r\n", 2) ||
     !stralloc_catb(&req->body, x, n) || !stralloc_catb(&req->body, "\r\n", 2))
    return 0;

  /* the loop is only woken up for writing when the request is already out */
  if(h->sent && h->sock != -1)
    io_wantwrite(h->sock);

  return 1;
}
