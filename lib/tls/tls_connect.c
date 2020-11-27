#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_connect(fd_t fd) {
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  if(tls_instance_return(i, SSL_connect(i->ssl)) == 1)
    return 1;
  if((errno = tls_instance_errno(i)))
    return -1;
  return 0;
}
#endif
