#include "../slist.h"
#include "../str.h"
#include "../alloc.h"
#include "../byte.h"

#include <stdlib.h>

void*
slist_pushb(slink** list, const void* x, size_t len) {
  slink* n;

  if((n = (slink*)alloc(sizeof(slink*) + len)) == NULL)
    return 0;

  while(*list) list = &(*list)->next;

  *list = n;

  n[0].next = NULL;
  byte_copy(((const char**)&n[1]), len, x);

  //  *((const char**)&n[1]) = str_dup(s);
  return n;
}
