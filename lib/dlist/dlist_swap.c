#include "../list.h"

void
list_swap(list* l, node* p1, node* p2) {
  node l1, l2;

  /* Return if its twice the same node */
  if(p1 == p2)
    return;

  l1 = *p1;
  l2 = *p2;

  if(l2.next)
    l2.next->prev = p1;
  else
    l->tail = p1;

  if(l2.prev)
    l2.prev->next = p1;
  else
    l->head = p1;

  if(l1.next)
    l1.next->prev = p2;
  else
    l->tail = p2;

  if(l1.prev)
    l1.prev->next = p2;
  else
    l->head = p2;

  l2.next = p1->next;
  l2.prev = p1->prev;
  l1.next = p2->next;
  l1.prev = p2->prev;

  *p1 = l1;
  *p2 = l2;
}
