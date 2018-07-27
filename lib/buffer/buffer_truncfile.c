#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#else
#include "../io_internal.h"
#endif

#include "../buffer.h"
#include "../open.h"

int
buffer_truncfile(buffer* b, const char* fn, char* y, size_t ylen) {
  if((b->fd = open_trunc(fn)) == -1) return -1;
  b->p = 0;
  b->n = 0;
  b->a = ylen;
  b->x = y;
  b->op = (void*)write;
  b->deinit = NULL;
  return 0;
}
