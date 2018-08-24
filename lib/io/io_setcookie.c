
#if defined(_WIN32) || defined(_WIN64)
#else
#endif
#include "../io_internal.h"

void io_setcookie(fd_t d, void* cookie) {
  io_entry* e;
  if((e = iarray_get(io_getfds(), d)))
    e->cookie = cookie;
}
