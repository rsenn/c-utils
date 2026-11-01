#include "../http.h"
#include "../alloc.h"

/* Releases what an http structure owns: the buffers, the requests and the
 * responses, the host. The connection is closed by http_close, which comes
 * first. The structure can be started over with http_init. */
void
http_free(http* h) {
  http_request* req;
  http_response* res;

  while((req = h->request)) {
    h->request = req->next;
    stralloc_free(&req->location);
    stralloc_free(&req->headers);
    stralloc_free(&req->body);
    alloc_free(req);
  }

  while((res = h->response)) {
    h->response = res->next;
    http_response_free(res);
  }

  if(h->q.in.x)
    alloc_free(h->q.in.x);
  if(h->q.out.x)
    alloc_free(h->q.out.x);

  h->q.in.x = h->q.out.x = NULL;
  h->q.in.p = h->q.in.n = h->q.out.p = h->q.out.n = 0;

  stralloc_free(&h->host);
}
