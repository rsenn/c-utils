#include "../list.h"

void
dlist_add_before(list* l, node* p, node* before) {
  /* If <before> is the list head, then a dlist_add_head() does the job */
  if(before == l->head) {
    dlist_unshift(l, p);
    return;
  }

  /* Make references on the new p */
  p->next = before;
  p->prev = before->prev;

  /* Update next-reference of the p before the <before> */
  before->prev->next = p;

  /* Update prev-reference of the <before> p */
  before->prev = p;
}
