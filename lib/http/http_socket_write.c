#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

ssize_t
http_socket_write(fd_t fd, void* buf, size_t len, void* b) {
  http* h = (http*)((buffer*)b)->cookie;
  ssize_t ret = 0;
#if DEBUG_OUTPUT
  buffer_putsflush(buffer_2, "http_socket_write\n");
#endif
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected)
      ret = http_ssl_connect(h);

    if(h->connected)
      ret = http_ssl_write(h->sock, buf, len, b);

  } else
#endif
    ret = winsock2errno(send(fd, buf, len, 0));

#if DEBUG_OUTPUT
  buffer_puts(buffer_2, "http_socket_write ");
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_putlong(buffer_2, errno);
    buffer_putm_internal(
        buffer_2, " (", errno == EAGAIN ? "EAGAIN" : errno == EWOULDBLOCK ? "EWOULDBLOCK" : "?", ")", 0);
  }
  buffer_putnlflush(buffer_2);
#endif

  return ret;
}
