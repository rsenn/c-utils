#include "../uint64.h"
#include "../buffer.h"
#include "../stralloc.h"
#include "../http.h"
#include "../fmt.h"

void
http_response_dump(http_response* r) {
#ifdef DEBUG_HTTP
  size_t len;

  buffer_putspad(buffer_2, "\x1b[38;5;69mhttp_response\x1b[0m ", 30);
  buffer_putspad(buffer_2, "\n\ttransfer", 18);
  buffer_puts(buffer_2, "HTTP_TRANSFER_");
  buffer_puts(buffer_2,
              ((const char* const[]){
                  "UNDEF",
                  "CHUNKED",
                  "LENGTH",
                  "BOUNDARY",
                  0,
              })[r->transfer]);
  buffer_putspad(buffer_2, "\n\tstatus", 18);
  buffer_puts(buffer_2,
              ((const char* const[]){
                  "-1",
                  "RECV_HEADER",
                  "RECV_DATA",
                  "STATUS_CLOSED",
                  "STATUS_ERROR",
                  "STATUS_BUSY",
                  "STATUS_FINISH",
                  0,
              })[r->status + 1]);
  buffer_putspad(buffer_2, "\n\tptr", 18);
  buffer_putulonglong(buffer_2, r->ptr);

  /* if(r->chnk != 0) {
     buffer_putspad(buffer_2, "\n\tchnk", 18);
     buffer_putulonglong(buffer_2, r->chnk);
   }*/

  /*if(r->line != 0) {
    buffer_putspad(buffer_2, "\n\tline", 18);
    buffer_putulonglong(buffer_2, r->line);
  }*/

  if(r->content_length != 0) {
    buffer_putspad(buffer_2, "\n\tcontent_length", 18);
    buffer_putulonglong(buffer_2, r->content_length);
  }

  if(stralloc_length(&r->boundary)) {
    buffer_putspad(buffer_2, "\n\tboundary", 18);
    buffer_putsa(buffer_2, &r->boundary);
  }

  if((len = r->data.len) > 0) {
    const char* x = r->data.s;
    buffer_putspad(buffer_2, "\n\tdata", 18);
    buffer_puts(buffer_2, "len: ");
    buffer_putulong(buffer_2, len);
    buffer_puts(buffer_2, " ");
    stralloc_dump(&r->data, buffer_2);

    /*buffer_puts(buffer_2, " \"");
    buffer_put_quoted(buffer_2, x, len);
    buffer_puts(buffer_2, " \"");*/
  }

  buffer_putnlflush(buffer_2);
#endif
}
