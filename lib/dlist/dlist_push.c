#include <stdlib.h>
#include "../dlist.h"

void
dlist_push(dlist* l, node* p) {
  /* We add to the dlist tail, so there's no next p */
  p->next = NULL;

  /* Previous p is the old tail */
  p->prev = l->tail;

  /* If there already is a p at the tail update
     its prev-reference, else update the head */
  if(l->tail)
    l->tail->next = p;
  else
    l->head = p;

  /* Now put the p to dlist tail */
  l->tail = p;
}
