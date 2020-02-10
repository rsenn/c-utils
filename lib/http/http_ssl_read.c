#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_read(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  char* msg = 0;
  int err = 0;
  ssize_t ret = 0;
  errno = 0;
  assert(h->tls);
  assert(h->connected);
do_read:

  if((ret = SSL_read(h->ssl, buf, len)) <= 0) {
    if((err = http_ssl_error(h, ret))) {
      if(http_ssl_io(h, err) <= -1)
        return -1;
    }
  }

#if DEBUG_OUTPUT
  buffer_puts(buffer_2, "http_ssl_read ");
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, http_ssl_errflag(err));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif