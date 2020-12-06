#include "../tls_internal.h"
#include "../io.h"
#include "../typedefs.h"
#include <assert.h>

int
tls_error(fd_t fd) {
  ssize_t ret;
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  return i->error;
}