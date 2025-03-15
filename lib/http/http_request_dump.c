#define NO_BUILTINS
#include "../buffer.h"
#include "../stralloc.h"
#include "../http.h"

void
http_request_dump(http_request* req) {
#ifdef DEBUG_HTTP
  size_t len;

  buffer_putspad(buffer_2, "\x1b[38;5;125mhttp_request\x1b[0m ", 30);
  buffer_putspad(buffer_2, "\n\tserial", 18);
  buffer_putulong(buffer_2, req->serial);
  buffer_putspad(buffer_2, "\n\ttype", 18);
  buffer_puts(buffer_2, (const char* const[]){"GET", "POST"}[req->type & 1]);
  buffer_putspad(buffer_2, "\n\tlocation", 18);
  buffer_putsa(buffer_2, &req->location);
  buffer_putspad(buffer_2, "\n\theaders", 18);
  buffer_putsa(buffer_2, &req->headers);
  buffer_putnlflush(buffer_2);
#endif
}
