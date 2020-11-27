#include "../tls_internal.h"

tls_instance*
tls_instance_get(fd_t fd) {
  return iarray_get(&tls_list, fd);
}
