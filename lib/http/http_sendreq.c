#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../http.h"
#include "../byte.h"
#include "../io.h"

static ssize_t
do_send(fd_t s, const void* buf, size_t len) {
  return send(s, buf, len, 0);
}

int
http_sendreq(http* h) {
  int ret;
  buffer* out = &h->q.out;
  if(h->request == NULL)
    return 0;
  buffer_puts(out, "GET ");
  buffer_putsa(out, &h->request->location);
  buffer_puts(out, " HTTP/1.1\r\n");
  buffer_puts(out, "Host: ");
  buffer_putsa(out, &h->host);
  buffer_puts(out, "\r\n");
  if(!h->keepalive)
    buffer_putm_internal(out, "Connection: ", h->keepalive ? "keep-alive" : "close", "\r\n", NULL);
  buffer_puts(out, "Accept: */*\r\n");
  buffer_puts(out, "Accept-Encoding: br, xz, lzma, bzip2, gzip, deflate\r\n");
  buffer_puts(out, "\r\n");
#if DEBUG_HTTP
  {
    const char* x = out->x;
    ssize_t i, n = out->p;
    while(n > 0) {
      i = byte_chr(x, n, '\r');
      buffer_puts(buffer_2, "Requst: ");
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
  ret = buffer_flush(out);
  if(ret != -1)
    h->sent = 1;
  return ret;
}