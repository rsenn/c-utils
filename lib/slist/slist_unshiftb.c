#include "../slist.h"
#include "../byte.h"
#include "../alloc.h"

int
slist_unshiftb(slink** list, const void* x, size_t len) {
  slink* n;

  if((n = (slink*)alloc(sizeof(slink*) + len)) == NULL)
    return 0;

  n->next = *list;
  *list = n;

  byte_copy(((const char**)&n[1]), len, x);
  return 1;
}
