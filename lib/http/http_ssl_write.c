#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_write(fd_t fd, const void* buf, size_t n, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret;
  int out = 0;
  errno = 0;
  if(!h->tls)
    return 0;
  if(!h->connected) {
    if((ret = http_ssl_connect(fd, h)) == 1) {
      io_wantread(fd);
      io_wantwrite(fd);
      errno = EWOULDBLOCK;
    }
    return -1;
  }
do_write:
  assert(h->connected);
  ret = SSL_write(h->ssl, buf, n);
  if(ret <= 0) {
    buffer_puts(buffer_2, "SSL write ");
    out = 1;
    ret = http_ssl_error(ret, h, 0);
  }
  if(ret == -1 && (errno == EWOULDBLOCK || errno == EAGAIN))
    return ret;
  if(out) {
    buffer_puts(buffer_2, "ret=");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);
  }
  return ret;
}
#endif