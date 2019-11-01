#include "../slist.h"

void
slist_add_before(slink** l, slink* p, slink* before) {
  while(*l && before && *l != before) l = &(*l)->next;

  p->next = *l;
  *l = p;
}
