#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../http.h"
#include "../byte.h"
#include "../io.h"
#include "../fmt.h"

static ssize_t
do_send(fd_t s, const void* buf, size_t len) {
  return send(s, buf, len, 0);
}

int
http_sendreq(http* h) {
  int ret;
  size_t len;
  buffer* out = &h->q.out;
  if(h->request == NULL)
    return 0;
  buffer_puts(out, "GET ");
  buffer_putsa(out, &h->request->location);
  buffer_puts(out, h->version == 11 ? " HTTP/1.1\r\n" : " HTTP/1.0\r\n");
  buffer_puts(out, "Host: ");
  buffer_putsa(out, &h->host);
  buffer_puts(out, "\r\n");
  if(!h->keepalive)
    buffer_putm_internal(out, "Connection: ", h->keepalive ? "keep-alive" : "close", "\r\n", NULL);
  buffer_puts(out, "Accept: */*\r\n");
  // buffer_puts(out, "Accept-Encoding: br, xz, lzma, bzip2, gzip,
  // deflate\r\n");
  buffer_puts(out, "\r\n");
#if DEBUG_HTTP
  {
    const char* x = out->x;
    ssize_t i, n = out->p;
    while(n > 0) {
      i = byte_chr(x, n, '\r');
      buffer_puts(buffer_2, "Request: ");
      buffer_put(buffer_2, x, i);
      buffer_putnlflush(buffer_2);
      x += i + 1;
      n -= i + 1;
      if(*x == '\n') {
        x++;
        n--;
      }
    }
  }
  buffer_flush(buffer_2);
#endif
  len = out->p;
#ifdef DEBUG_HTTP

  buffer_putspad(buffer_2, "http_sendreq ", 30);
  buffer_puts(buffer_2, "location=");
  buffer_putsa(buffer_2, &h->request->location);

  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }
  if(len > 0) {
    buffer_puts(buffer_2, " len=");
    buffer_putlong(buffer_2, len);
  }
  if(out->n > out->p) {
    buffer_puts(buffer_2, " code=");
    buffer_put_escaped(buffer_2, out->x, out->p, &fmt_escapecharnonprintable);
  }
  buffer_puts(buffer_2, " status=");
  buffer_puts(buffer_2,
              ((const char* const[]){"-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status + 1]);
  buffer_putnlflush(buffer_2);
  buffer_flush(buffer_2);
#endif
  buffer_flush(out);
  ret = len;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_sendreq ", 30);
  buffer_puts(buffer_2, "ret=");
  buffer_putlong(buffer_2, ret);
  if(ret < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, ret));
  }
  buffer_putnlflush(buffer_2);
#endif
  if(ret != -1) {
    h->sent = 1;
    h->response->status = HTTP_RECV_HEADER;

    io_onlywantread(h->sock);
  }
  return ret;
}