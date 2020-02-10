#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_read(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  char* msg = 0;
  ssize_t ret = 0;

  if(h->tls) {
    if(!h->connected) {
      if((ret = http_ssl_connect(fd, h)) == 1) {
        h->connected = 1;
        io_dontwantread(fd);
        io_wantwrite(fd);
      }
      errno = EAGAIN;
      return -1;
    }
  }
  assert(h->connected);
  if((ret = SSL_read(h->ssl, buf, len)) <= 0) {
    ret = http_ssl_error(ret, h, 0);
    if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
      return ret;
  }
  buffer_puts(buffer_2, "SSL read = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
  return ret;
}
#endif