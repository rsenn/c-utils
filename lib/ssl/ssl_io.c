#include "../ssl_internal.h"
#include "../io.h"
#include <assert.h>

void
ssl_io(fd_t fd) {
  ssize_t ret;
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);

  i->wantread = &io_onlywantread;
  i->wantwrite = &io_onlywantwrite;
}
