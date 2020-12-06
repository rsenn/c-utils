#include "../tls_internal.h"

tls_instance*
tls_instance_get(fd_t fd) {
  tls_instance* inst = iarray_get(&tls_list, fd);

  return inst->ssl ? inst : 0;
}
