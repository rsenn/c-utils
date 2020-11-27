#include "../tls_internal.h"
#include "../byte.h"
#include <assert.h>

tls_instance*
tls_instance_new(fd_t fd) {
  tls_instance* i;
  if((i = iarray_allocate(&tls_list, fd))) {
    assert(i->ssl == 0);
    byte_zero(i, sizeof(tls_instance));
  }
  return i;
}