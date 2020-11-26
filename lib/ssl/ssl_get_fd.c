#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssl_t*
ssl_get_fd(fd_t fd) {
  ssl_instance* inst = iarray_get(&ssl_list, fd);
  assert(inst);
  assert(inst->ssl);
  return inst->ssl;
}
#endif