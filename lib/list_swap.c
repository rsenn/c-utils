#include "list.h"

void
list_swap(list *l, link *link1, link *link2) {
  link n1;
  link n2;

  /* Return if its twice the same link */
  if(link1 == link2)
    return;

  n1 = *link1;
  n2 = *link2;

  if(n2.next)
    n2.next->prev = link1;
  else
    l->tail = link1;

  if(n2.prev)
    n2.prev->next = link1;
  else
    l->head = link1;

  if(n1.next)
    n1.next->prev = link2;
  else
    l->tail = link2;

  if(n1.prev)
    n1.prev->next = link2;
  else
    l->head = link2;

  n2.next = link1->next;
  n2.prev = link1->prev;
  n1.next = link2->next;
  n1.prev = link2->prev;

  *link1 = n1;
  *link2 = n2;
}

