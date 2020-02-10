#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#include <errno.h>

int
http_ssl_errno(int err) {
  ssize_t ret = 0;
  if(err == SSL_ERROR_SSL) {
    ret = 0;
  } else if(err == SSL_ERROR_WANT_WRITE || err == SSL_ERROR_WANT_READ) {
    ret = err == SSL_ERROR_WANT_READ ? EAGAIN : EWOULDBLOCK;
  }
  return ret;
}

ssize_t
http_ssl_io_errhandle(http* h, int err) {
  ssize_t r = 0;
  int e = 0;
  if(err == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    err = errno == EAGAIN ? SSL_ERROR_WANT_READ : SSL_ERROR_WANT_WRITE;
  else if(err == EAGAIN || err == EWOULDBLOCK)
    err = err == EAGAIN ? SSL_ERROR_WANT_READ : SSL_ERROR_WANT_WRITE;
  if(err == SSL_ERROR_WANT_WRITE || err == SSL_ERROR_WANT_READ) {
    r = -1;
    e = err == SSL_ERROR_WANT_READ ? EAGAIN : EWOULDBLOCK;
  } else if(err == SSL_ERROR_SSL || err == 0) {
    e = 0;
    r = 1;
  }
  errno = e;
  return r;
}

ssize_t
http_ssl_io_again(http* h, ssize_t ret) {
  if(ret == -1) {
    if(errno == EAGAIN) {
      io_dontwantwrite(h->sock);
      io_wantread(h->sock);
      return -1;
    } else if(errno == EWOULDBLOCK) {
      io_dontwantread(h->sock);
      io_wantwrite(h->sock);
      return -1;
    }
  }
  return 0;
}
#endif