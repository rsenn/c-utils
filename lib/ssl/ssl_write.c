#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
ssl_write(fd_t fd, const void* data, size_t len) {
  ssize_t ret;
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);

  ret = ssl_instance_return(i, SSL_write(i->ssl, data, len));

  return ret;
}
#endif
