#include "../slist.h"

void
slist_add_before(slist *l, slink *p, slink *before) {
  slink** ptr = &l->root;

  while(*ptr && *ptr != before)
    ptr = &(*ptr)->next;
  
  p->next = *ptr;
  *ptr = p;
}
