#include "../byte.h"
#include "../stralloc.h"
#include <stdlib.h>

char*
stralloc_moveb(stralloc* from, size_t* len) {
  char* s;

  if(len)
    *len = from->len;

  s = from->s;

  from->s = NULL;
  from->len = 0;
  from->a = 0;

  return s;
}
