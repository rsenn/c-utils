#include <stdlib.h>
#include "../list.h"

void
list_unshift(list *l, link *p) {
  /* We add to the list head, so there's no previous p */
  p->prev = NULL;

  /* Next p is the old head */
  p->next = l->head;

  /* If there already is a p at the head update
     its prev-reference, else update the tail */
  if(l->head)
    l->head->prev = p;
  else
    l->tail = p;

  /* Now put the p to list head */
  l->head = p;
}

