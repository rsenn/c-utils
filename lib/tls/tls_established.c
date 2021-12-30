#include "../tls_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_established(fd_t fd) {
  int ret;
  tls_instance_t* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  ret = SSL_is_init_finished(i->ssl);
#ifdef DEBUG_TLS
  buffer_putspad(buffer_2, "tls_established ", 30);
  buffer_puts(buffer_2, "ret=");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif
