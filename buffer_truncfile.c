#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

#include "open.h"
#include "buffer.h"

int buffer_truncfile(buffer *b, const char *fn, char *y, size_t ylen)
{
  if((b->fd = open_trunc(fn)) == -1) return -1;
  b->p=0; b->n = 0;
  b->a=ylen; b->x=y;
  b->op = (void*)write;
  b->todo = NOTHING;
  return 0;
}
