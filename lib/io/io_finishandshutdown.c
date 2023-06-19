#include "../io_internal.h"

void
io_finishandshutdown(void) {
  iarray_free((iarray*)io_getfds());
  array_reset(&io_pollfds);
}
