#include "../ssl_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
ssl_read(fd_t fd, void* data, size_t len) {
  ssize_t ret;
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);

  if(!SSL_is_init_finished(i->ssl)) {
    if((ret = ssl_instance_handshake(i)) != 1)
      return ret;
  }

  if((ret = ssl_instance_return(i, SSL_read(i->ssl, data, len))) <= 0)
    errno = ssl_instance_errno(i);

  return ret;
}
#endif
