#include "../ssl_internal.h"
#include <errno.h>

#ifdef HAVE_OPENSSL
int
ssl_instance_errno(ssl_instance* i) {
  int ret;
  switch(i->error) {
    case SSL_ERROR_WANT_WRITE: ret = EWOULDBLOCK; break;
    case SSL_ERROR_WANT_READ: ret = EAGAIN; break;
    case SSL_ERROR_ZERO_RETURN:
    case SSL_ERROR_NONE: ret = 0; break;
    default: ret = EIO; break;
  }
  return ret;
}
#endif
