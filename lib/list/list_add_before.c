#include "../list.h"

void
list_add_before(list *l, link *p, link *before) {
  /* If <before> is the list head, then a list_add_head() does the job */
  if(before == l->head) {
    list_unshift(l, p);
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
