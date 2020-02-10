#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
http_ssl_error(ssize_t ret, http* h, char** mptr) {
  /* get error code */
  char buf[256];
  size_t n = sizeof(buf);
  int err;

  if(ret >= 0)
    return ret;
  /* get error code */
  err = SSL_get_error(h->ssl, ret);

  ERR_error_string_n(err, buf, n);
  ERR_clear_error();

  if(mptr)
    *mptr = str_dup(buf);

  /* call ssl_read() again when socket gets readable */
  if(err == SSL_ERROR_WANT_READ) {
    io_wantread(h->sock);
    errno = EAGAIN;
    buffer_putsflush(buffer_2, "SSL want read\n");

    return -1;
    /* call ssl_read() again when socket gets writeable */
  } else if(err == SSL_ERROR_WANT_WRITE) {
    io_wantwrite(h->sock);
    errno = EWOULDBLOCK;
    buffer_putsflush(buffer_2, "SSL want write\n");
    return -1;
  } else if(err == SSL_ERROR_SYSCALL) {
    /* ignore these */
    if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN) {
      // errno = EAGAIN;
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
    buffer_puts(buffer_2, "SSL ret: ");
    buffer_putlong(buffer_2, ret);
    buffer_puts(buffer_2, " err: ");
    if(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE)
      buffer_puts(buffer_2, err == SSL_ERROR_WANT_READ ? "want-read" : "want-write");
    else
      buffer_putlong(buffer_2, err);
    buffer_puts(buffer_2, " : ");
    buffer_puts(buffer_2, buf);
    buffer_putnlflush(buffer_2);
  }

  return ret;
}
#endif