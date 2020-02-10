#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <assert.h>
#include <openssl/err.h>

ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret;
  char* msg = 0;
  errno = 0;
  assert(!h->connected);
  ret = SSL_connect(h->ssl);
  if(ret <= 0) {
    buffer_puts(buffer_2, "http_ssl_connect ");
    ret = http_ssl_error(ret, h, 0);
  }

  if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    if(errno == EAGAIN) {
      io_wantread(fd);
    } else if(errno == EWOULDBLOCK) {
      io_wantwrite(fd);
    }
  } else if(ret == 1) {
    h->connected = 1;
    io_wantwrite(fd);
  }
  return ret;
}
#endif