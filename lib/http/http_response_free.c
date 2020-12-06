#include "../http.h"
#include "../alloc.h"

void
http_response_free(http_response* r) {
  stralloc_free(&r->data);
  stralloc_free(&r->boundary);
  alloc_free(r);
}