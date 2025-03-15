#define NO_BUILTINS
#include "../uint64.h"
#include "../buffer.h"
#include "../http.h"
#include "../fmt.h"
#include "../unix.h"

void
http_dump(http* h) {
#ifdef DEBUG_HTTP
  buffer_puts(buffer_2, " s=");
  buffer_putlong(buffer_2, h->sock);

  if(h->tls) {
    buffer_puts(buffer_2, " tls=");
    buffer_putlong(buffer_2, !!h->tls);
  }

  if(h->connected) {
    buffer_puts(buffer_2, " connected=");
    buffer_putlong(buffer_2, !!h->connected);
  }

  if(h->keepalive) {
    buffer_puts(buffer_2, " keepalive=");
    buffer_putlong(buffer_2, !!h->keepalive);
  }

  if(h->nonblocking) {
    buffer_puts(buffer_2, " nonblocking=");
    buffer_putlong(buffer_2, !!h->nonblocking);
  }

  if(h->sent) {
    buffer_puts(buffer_2, " sent=");
    buffer_putlong(buffer_2, !!h->sent);
  }

  if(h->err) {
    buffer_puts(buffer_2, " h->err=");
    buffer_puts(buffer_2, unix_errno(h->err));
  }

  if(h->response) {

    buffer_puts(buffer_2, " response={");

    if(h->response->code != -1) {
      buffer_puts(buffer_2, " code=");
      buffer_putlong(buffer_2, h->response->code);
    }
    buffer_puts(buffer_2, " err=");
    buffer_putlong(buffer_2, h->response->err);
    buffer_puts(buffer_2, " data_pos=");
    buffer_putlong(buffer_2, h->response->data_pos);

    if(h->response->data.len > 0) {
      buffer_puts(buffer_2, " data='");

      size_t n = h->response->data.len;

      buffer_putfmt(buffer_2, h->response->data.s, n > 100 ? 100 : n, &fmt_escapecharnonprintable);
      buffer_puts(buffer_2, n > 100 ? "'..." : "'");
    }

    if(h->response->content_length > 0) {
      buffer_puts(buffer_2, " content_length=");
      buffer_putlong(buffer_2, h->response->content_length);
    }
    if(h->response->chunk_length > 0) {
      buffer_puts(buffer_2, " chunk_length=");
      buffer_putlonglong(buffer_2, h->response->chunk_length);
    }
    buffer_puts(buffer_2, " transfer=");
    buffer_puts(buffer_2, "HTTP_TRANSFER_");
    buffer_puts(buffer_2,
                ((const char* const[]){
                    "UNDEF",
                    "CHUNKED",
                    "LENGTH",
                    "BOUNDARY",
                    0,
                })[h->response->transfer]);

    buffer_puts(buffer_2, " status=");
    buffer_puts(buffer_2,
                ((const char* const[]){
                    "0", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status]);

    buffer_puts(buffer_2, " }");
  }

  buffer_putnlflush(buffer_2);
#endif
}
