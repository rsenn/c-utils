#include "../tls_internal.h"

tls_instance_t*
tls_instance_get(fd_type fd) {
  tls_instance_t* inst = iarray_get(&tls_list, fd);

  return inst->ssl ? inst : 0;
}
