#include "../ssl_internal.h"

ssl_instance*
ssl_instance_get(fd_t fd) {
  return iarray_get(&ssl_list, fd);
}
