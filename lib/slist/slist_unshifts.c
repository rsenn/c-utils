#include "../slist.h"
#include "../str.h"
#include "../alloc.h"

int
slist_unshifts(slink** list, const char* s) {
  slink* n;

  if((n = (slink*)alloc(sizeof(slink*) + sizeof(char*))) == NULL)
    return 0;

  n->next = *list;
  *list = n;

  *((const char**)&n[1]) = str_dup(s);
  return 1;
}
