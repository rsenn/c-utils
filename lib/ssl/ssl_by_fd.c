#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssl_t*
ssl_by_fd(fd_t fd) {
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);
  return i->ssl;
}
#endif