#include "../list.h"

void
list_add_after(list *l, link *p, link *after) {
  /* If <after> is the list tail, then a list_add_tail() does the job */
  if(after == l->tail) {
    list_push(l, p);
    return;
  } 

  /* Make references on the new p */
  p->next = after->next;
  p->prev = after;

  /* Update prev-reference of the p after the <after> p */
  after->next->prev = p;

  /* Update next-reference of the <after> p */
  after->next = p;
} 
