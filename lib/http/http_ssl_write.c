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
  assert(h->tls);
  assert(h->connected);
do_write:
  ret = SSL_write(h->ssl, buf, n);
  if(ret <= 0) {
    //   buffer_puts(buffer_2, "http_ssl_write ");
    int err = http_ssl_error(h, ret);
  }
  return ret;
}
#endif