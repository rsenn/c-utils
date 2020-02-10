#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_write(fd_t fd, const void* buf, size_t n, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret;
  errno = 0;
  if(!h->tls)
    return 0;
  if(!h->connected) {
    if((ret = http_ssl_connect(fd, h)) == 1)
      h->connected = 1;
    if(h->connected) {
      io_wantwrite(fd);
      if(!io_canwrite(fd)) {
        errno = EWOULDBLOCK;
        return -1;
      }
    }
    return ret;
  }
do_write:
  assert(h->connected);
  ret = SSL_write(h->ssl, buf, n);
  if(ret <= 0) {
    buffer_puts(buffer_2, "http_ssl_write ");
    ret = http_ssl_error(ret, h, 0);
  }
  return ret;
}
#endif