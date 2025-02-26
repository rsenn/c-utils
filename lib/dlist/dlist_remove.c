#include <stdlib.h>
#include "../dlist.h"

void
dlist_remove(dlist* l, node* p) {
  /* If there is a prev p, update its next-
     reference, otherwise update the head */

  if(p == l->head)
    l->head = p->next;
  else
    p->prev->next = p->next;

  /* If there is a next p, update its prev-
     reference otherwise update the tail */

  if(l->tail == p)
    l->tail = p->prev;
  else
    p->next->prev = p->prev;

  /* Zero references on this p */
  p->next = NULL;
  p->prev = NULL;
}
