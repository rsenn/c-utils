#include "../ssl_internal.h"
#include "../byte.h"
#include <assert.h>

ssl_instance*
ssl_instance_new(fd_t fd) {
  ssl_instance* ret;
  if((ret = iarray_allocate(&ssl_list, fd))) {
    assert(ret->ssl == 0);
    byte_zero(ret, sizeof(ssl_instance));
  }
  return ret;
}