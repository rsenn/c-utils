#include "../tokenizer_internal.h"
#include "../seek.h"

int64
tokenizer_ftello(tokenizer* t) {
  buffer* b = t->input;
  int64 ret = 0;
  if(b->fd)
    ret += seek_cur(b->fd);
  ret += b->p;
  return ret;
}
