#include "../dlist.h"

node*
dlist_find_delete(dlist* l, int (*pred)(void*)) {
  node* p;

  /* Loop through all links until we find the pointer */
  dlist_foreach(l, p) {
    if(pred(&p[1])) {
      /* If there is a prev node, update its next-
         reference, otherwise update the head */

      if(p->prev)
        p->prev->next = p->next;
      else
        l->head = p->next;

      /* If there is a next node, update its prev-
         reference otherwise update the tail */

      if(p->next)
        p->next->prev = p->prev;
      else
        l->tail = p->prev;

      /* Zero references on this node */
      p->next = NULL;
      p->prev = NULL;

      return p;
    }
  }

  return NULL;
}
