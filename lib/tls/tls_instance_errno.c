#include "../tls_internal.h"
#include <errno.h>

#ifdef HAVE_OPENSSL
int
tls_instance_errno(tls_instance_t* i) {
  switch(i->error) {
    case SSL_ERROR_WANT_WRITE: return EWOULDBLOCK;
    case SSL_ERROR_WANT_READ: return EAGAIN;
    case SSL_ERROR_ZERO_RETURN:
    case SSL_ERROR_NONE: return 0;
    case SSL_ERROR_SYSCALL: return i->syserr;
    default: return EIO;
  }
}
#endif
