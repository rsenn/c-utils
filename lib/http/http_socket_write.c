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
  http* h = ((buffer*)b)->cookie;
  ssize_t ret = 0;

#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      if((ret = http_ssl_connect(h->sock, h)) == -1)
        return -1;
    }

    if(!io_canwrite(h->sock)) {
      errno = EWOULDBLOCK;
      return -1;
    }
  }
#endif

#ifdef HAVE_OPENSSL
  if(h->ssl) {
    ret = http_ssl_write(h->sock, buf, len, b);
    buffer_puts(buffer_2, "SSL write = ");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);
  }
#endif
  else
    ret = winsock2errno(send(fd, buf, len, 0));

  return ret;
}