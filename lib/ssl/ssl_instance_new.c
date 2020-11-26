#include "../ssl_internal.h"
#include "../byte.h"
#include <assert.h>

ssl_instance*
ssl_instance_new(fd_t fd) {
  ssl_instance* i;
  if((i = iarray_allocate(&ssl_list, fd))) {
    assert(i->ssl == 0);
    byte_zero(i, sizeof(ssl_instance));
  }
  return i;
}