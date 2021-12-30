#include "../io.h"
#include "../typedefs.h"
#include "../tls_internal.h"
#include <assert.h>

int
tls_error(fd_t fd) {
  ssize_t ret;
  tls_instance_t* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  return i->error;
}
