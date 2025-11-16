#define _WINSOCKAPI_
#include "../io.h"
#include "../typedefs.h"
#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
tls_err_t
tls_error(fd_type fd) {
  ssize_t ret;
  tls_instance_t* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  return i->error;
}
#endif
