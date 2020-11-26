#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
ssl_established(fd_t fd) {
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);

  return SSL_is_init_finished(i->ssl);
}
#endif
