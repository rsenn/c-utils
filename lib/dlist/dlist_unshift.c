#include <stdlib.h>
#include "../dlist.h"

void
dlist_unshift(dlist* l, node* p) {
  /* We add to the dlist head, so there's no previous p */
  p->prev = NULL;

  /* Next p is the old head */
  p->next = l->head;

  /* If there already is a p at the head update
     its prev-reference, else update the tail */

  if(l->head)
    l->head->prev = p;
  else
    l->tail = p;

  /* Now put the p to dlist head */
  l->head = p;
}
