#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_write(fd_t fd, const void* buf, size_t n, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret;
  int err;
  errno = 0;
  assert(h->tls);
  assert(h->connected);
do_write:
  if((ret = SSL_write(h->ssl, buf, n)) <= 0) {
    if((err = http_ssl_error(h, ret))) {
      if(http_ssl_io(h, err) <= -1)
        return -1;
    }
  }

#if DEBUG_OUTPUT
  buffer_puts(buffer_2, "http_ssl_write ");
  buffer_puts(buffer_2, " ret=");
  buffer_putulong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, http_ssl_errflag(err));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif