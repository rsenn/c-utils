#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

tls_t*
tls_by_fd(fd_t fd) {
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);
  return i->ssl;
}
#endif