#include "../slist.h"

void
slist_add_after(slink** l, slink* p, slink* after) {
  if(after == NULL) {
    while(*l) l = &(*l)->next;

    *l = p;
    p->next = NULL;
  } else {
    p->next = after->next;
    after->next = p;
  }
}
