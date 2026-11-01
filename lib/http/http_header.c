#include "../http.h"
#include "../str.h"

/* adds "name: value" to the request that was started last, 0 on failure */
int
http_header(http* h, const char* name, const char* value) {
  http_request* req = h->request;

  if(req == NULL)
    return 0;

  return stralloc_cats(&req->headers, name) && stralloc_catb(&req->headers, ": ", 2) &&
         stralloc_cats(&req->headers, value) && stralloc_catb(&req->headers, "\r\n", 2);
}
