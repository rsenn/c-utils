#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void
tls_free(fd_type fd) {
  tls_instance_t* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);
  assert(i->bio);
  SSL_free(i->ssl);
  i->ssl = 0;
  BIO_free(i->bio);
  i->bio = 0;
}
#endif
