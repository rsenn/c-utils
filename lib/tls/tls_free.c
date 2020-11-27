#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void
tls_free(fd_t fd) {
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  SSL_free(i->ssl);
  i->ssl = 0;
}
#endif
