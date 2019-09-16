#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../http.h"
#include "../io_internal.h"

static ssize_t
do_send(fd_t s, const void* buf, size_t len) {
  return send(s, buf, len, 0);
}

int
http_sendreq(http* h) {
  buffer* out = &h->q.out;
  if(h->request == NULL)
    return 0;

  buffer_puts(out, "GET ");
  buffer_putsa(out, &h->request->location);
  buffer_puts(out, " HTTP/1.1\r\n");

  buffer_puts(out, "Host: ");
  buffer_putsa(out, &h->host);
  buffer_puts(out, "\r\n");

  // buffer_puts(out, "Transfer-Encoding: chunked\r\n");
  // buffer_puts(out, "Connection: close\r\n");
  buffer_puts(out, "Connection: keep-alive\r\n");

  buffer_putsflush(out, "\r\n");
  buffer_flush(out);

  io_dontwantwrite(h->sock);
  /*
    buffer_freshen(&h->q.in);
    http_readable(h, 0);
  */
  io_wantread(h->sock);

  return 1;
}
