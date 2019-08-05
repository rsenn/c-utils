#include "../io_internal.h"

void
io_finishandshutdown(void) {
  iarray_free(io_getfds());
  array_reset(&io_pollfds);
}

