#include "../ssl_internal.h"
#include "../io.h"
#include <assert.h>
 
void
ssl_io(fd_t fd) {
  ssize_t ret;
  ssl_instance* inst = iarray_get(&ssl_list, fd);
  assert(inst);
  
  inst->wantread = &io_onlywantread;
  inst->wantwrite= &io_onlywantwrite;
}
