#include "../http.h"
#include "../buffer.h"
#include "../fmt.h"

void
http_response_dump(http_response* r) {
  size_t len;
  const char* x;
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[38;5;69mhttp_response\x1b[0m ", 30);
  buffer_putspad(buffer_2, "\n\ttransfer", 18);
  buffer_puts(buffer_2, ((const char* const[]){"HTTP_TRANSFER_UNDEF", "HTTP_TRANSFER_CHUNKED", "HTTP_TRANSFER_LENGTH", "HTTP_TRANSFER_BOUNDARY", 0})[r->transfer]);
  buffer_putspad(buffer_2, "\n\tstatus", 18);
  buffer_puts(buffer_2, ((const char* const[]){"-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[r->status + 1]);
  buffer_putspad(buffer_2, "\n\tptr", 18);
  buffer_putulonglong(buffer_2, r->ptr);
  buffer_putspad(buffer_2, "\n\tchnk", 18);
  buffer_putulonglong(buffer_2, r->chnk);
  buffer_putspad(buffer_2, "\n\tline", 18);
  buffer_putulonglong(buffer_2, r->line);
  buffer_putspad(buffer_2, "\n\tcontent_length", 18);
  buffer_putulonglong(buffer_2, r->content_length);

  if(stralloc_length(&r->boundary)) {
    buffer_putspad(buffer_2, "\n\tboundary", 18);
    buffer_putsa(buffer_2, &r->boundary);
  }
  if((len = stralloc_length(&r->data)) > 0) {
    x = stralloc_begin(&r->data);
    buffer_putspad(buffer_2, "\n\tdata", 18);
    buffer_puts(buffer_2, "len: ");
    buffer_putulonglong(buffer_2, len);
    buffer_puts(buffer_2, " \"");
    buffer_put_escaped(buffer_1, x, len, &fmt_escapecharnonprintable);
    buffer_puts(buffer_2, " \"");
  }
  buffer_putnlflush(buffer_2);
#endif
}