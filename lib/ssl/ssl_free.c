#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void
ssl_free(fd_t fd) {
  ssl_instance* inst = iarray_get(&ssl_list, fd);
  assert(inst);
  assert(inst->ssl);

  SSL_free(inst->ssl);
  inst->ssl = 0;
}
#endif
