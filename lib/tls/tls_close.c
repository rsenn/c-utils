#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_close(fd_t fd) {
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  return tls_instance_return(i, SSL_shutdown(i->ssl));
}
#endif
