#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_accept(fd_t fd) {
  int ret;
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  if((ret = tls_instance_return(i, TLS_OP_ACCEPT, SSL_accept(i->ssl))) < 0)
    errno = tls_instance_errno(i);

  return ret;
}
#endif
