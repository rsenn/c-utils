#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#include <errno.h>

ssize_t
http_ssl2errno(void* ssl, ssize_t ret) {

  int sslerr;
  errno = 0;

  if(ret <= 0) {
    sslerr = SSL_get_error(ssl, ret);

    switch(sslerr) {
      case SSL_ERROR_WANT_READ: errno = EAGAIN; break;
      case SSL_ERROR_WANT_WRITE: errno = EWOULDBLOCK; break;
      case SSL_ERROR_SYSCALL: errno = EIO; break;
      case SSL_ERROR_ZERO_RETURN: errno = EIO; break;
      case SSL_ERROR_SSL: errno = EIO; break;
      case SSL_ERROR_NONE: errno = 0; break;
      default: break;
    }
  }
  return ret;
}
/*
http_return_value
http_ssl2errno(void* ssl, ssize_t r) {

  http_return_value v = {0, 0};

  int sslerr = SSL_get_error(ssl, r);

  switch(sslerr) {
    case SSL_ERROR_WANT_READ: {
      ret = -1;
      errno = EAGAIN;
      break;
    }
    case SSL_ERROR_WANT_WRITE: {
      ret = -1;
      errno = EWOULDBLOCK;
      break;
    }
    case SSL_ERROR_SYSCALL: {
      ret = -1;
      errno = EIO;
      break;
    }
    case SSL_ERROR_ZERO_RETURN: {
      ret = 0;
      errno = EIO;
      break;
    }

    case SSL_ERROR_SSL: {
      ret = -1;
      errno = 0;
      break;
    }
    case SSL_ERROR_NONE: {
      ret = 1;
      errno = 0;
      break;
    }
    default: {
      ret = -1;
      break;
    }
  }
  return v;
}*/
#endif