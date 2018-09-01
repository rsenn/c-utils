#include "../slist.h"

void
slist_push(slist *l, slink *p) {
  slink** ptr = &l->root;

  while(*ptr)
    ptr = &(*ptr)->next;

  *ptr = p;
}
