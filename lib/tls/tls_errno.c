#include "../tls_internal.h"
#include "../io.h"
#include "../typedefs.h"
#include <assert.h>

int
tls_errno(fd_type fd) {
  tls_instance_t* i = tls_instance_get(fd);
  assert(i);
  return tls_instance_errno(i);
}
