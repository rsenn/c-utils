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

void
stralloc_move(stralloc* to, stralloc* from) {
  /*if(to->a == 0)
    to->s = NULL;*/

  //stralloc_free(to);

  to->s = from->s;
  to->a = from->a;
  to->len = from->len;

  stralloc_init(from);
}
