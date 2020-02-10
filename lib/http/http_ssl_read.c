#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
http_ssl_read(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret = SSL_read(h->ssl, buf, len);
  char* msg = 0;
  if(ret <= 0) {
    ret = http_ssl_error(ret, h, &msg);
    if(ret == -1 && errno == EAGAIN)
      return ret;
  }
  buffer_puts(buffer_2, "SSL read = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
  return ret;
}
#endif