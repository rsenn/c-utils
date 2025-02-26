#include "../dlist.h"

void
dlist_add_after(dlist* l, node* p, node* after) {
  /* If <after> is the dlist tail, then a dlist_add_tail() does the job */

  if(after == l->tail) {
    dlist_push(l, p);
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
