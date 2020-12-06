#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_close(fd_t fd) {
  ssize_t ret;
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);
  if((ret = tls_instance_return(i, TLS_OP_SHUTDOWN, SSL_shutdown(i->ssl))) < 0)
    errno = tls_instance_errno(i);
  return ret;
}
#endif
