#include "../slist.h"

void
slist_add_after(slist *l, slink *p, slink *after) {
  p->next = after->next;
  after->next = p;
}
