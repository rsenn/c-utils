#include "http.h"
#include "buffer.h"
#include <sys/socket.h>

static ssize_t
do_send(int64 s, const void* buf, size_t len) {
  return send(s, buf, len, 0);
}

int
http_sendreq(http* h) {
  char buf[1024];
  buffer b = BUFFER_INIT(do_send, h->sock, buf, sizeof(buf));

  if(h->request == NULL)
    return 0;

  buffer_puts(&b, "GET ");
  buffer_putsa(&b, &h->request->location);
  buffer_puts(&b, " HTTP/1.1\r\n");
  buffer_puts(&b, "Host: ");
  buffer_putsa(&b, &h->host);
  buffer_puts(&b, "\r\n");
  buffer_putsflush(&b, "\r\n");
  buffer_flush(&b);
  return 1;
}
