#include "../http.h"
#include "../byte.h"
#include "../str.h"

/* adds "name: value" to the request that was started last, 0 on failure or
 * if name or value has a CR or LF in it, which would add more headers or
 * end them */
int
http_header(http* h, const char* name, const char* value) {
  http_request* req = h->request;

  if(req == NULL || name[0] == '\0' || str_chr(name, ':') != str_len(name))
    return 0;

  if(str_chrs(name, "\r\n", 2) != str_len(name) || str_chrs(value, "\r\n", 2) != str_len(value))
    return 0;

  return stralloc_cats(&req->headers, name) && stralloc_catb(&req->headers, ": ", 2) &&
         stralloc_cats(&req->headers, value) && stralloc_catb(&req->headers, "\r\n", 2);
}
