#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
http_ssl_error(ssize_t ret, http* h, char** mptr) {
  char buf[256];
  size_t n = sizeof(buf);
  int err;
  if(ret >= 0)
    return ret;
  err = SSL_get_error(h->ssl, ret);
  ERR_error_string_n(err, buf, n);
  ERR_clear_error();
  if(err == SSL_ERROR_WANT_READ) {
    io_wantread(h->sock);
    errno = EAGAIN;
    ret = -1;
  } else if(err == SSL_ERROR_WANT_WRITE) {
    io_wantwrite(h->sock);
    errno = EWOULDBLOCK;
    ret = -1;
  } else if(err == SSL_ERROR_SYSCALL) {
    if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN) {
    }
  } else if(err == SSL_ERROR_ZERO_RETURN) {
    ret = 0;
  } else if(err == SSL_ERROR_SSL) {
    io_wantwrite(h->sock);
    ret = 1;
  } else {
    ret = -1;
  }
  if(ret <= 0) {
    buffer_puts(buffer_2, " ret: ");
    buffer_putlong(buffer_2, ret);

    if(err) {
      buffer_puts(buffer_2, " err: ");
      buffer_putlong(buffer_2, err);

      if(buf[0]) {
        buffer_puts(buffer_2, " : ");
        buffer_puts(buffer_2, buf);
      }
    }
  }
  if(ret >= 0)
    errno = 0;
  return ret;
}
#endif
