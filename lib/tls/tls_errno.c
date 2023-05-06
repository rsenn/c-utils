#include "../io.h"
#include "../typedefs.h"
#include "../tls_internal.h"
#include <assert.h>

int
tls_errno(fd_t fd) {
   tls_instance_t* i = tls_instance_get(fd);
  assert(i);
  return tls_instance_errno(i);
}
