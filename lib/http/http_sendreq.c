#include "../buffer.h"
#include "../http.h"
#include "../io_internal.h"
#include "../socket.h"
static ssize_t
do_send(fd_t s, const void* buf, size_t len) {
  return send(s, buf, len, 0);
}

int
http_sendreq(http* h) {
  if(h->request == NULL) return 0;
  buffer_puts(&h->q.out, "GET ");
  buffer_putsa(&h->q.out, &h->request->location);
  buffer_puts(&h->q.out, " HTTP/1.1\r\n");
  buffer_puts(&h->q.out, "Host: ");
  buffer_putsa(&h->q.out, &h->host);
  buffer_puts(&h->q.out, "\r\n");
  buffer_putsflush(&h->q.out, "\r\n");
  buffer_flush(&h->q.out);
  return 1;
}
