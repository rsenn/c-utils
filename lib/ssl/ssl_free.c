#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void
ssl_free(fd_t fd) {
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);

  SSL_free(i->ssl);
  i->ssl = 0;
}
#endif
