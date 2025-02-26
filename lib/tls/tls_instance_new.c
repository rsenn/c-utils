#include "../tls_internal.h"
#include "../byte.h"
#include <assert.h>

tls_instance_t*
tls_instance_new(fd_type fd) {
  tls_instance_t* i;

  if((i = iarray_allocate(&tls_list, fd))) {
    assert(i->ssl == 0);
    byte_zero(i, sizeof(tls_instance_t));
  }
  return i;
}
