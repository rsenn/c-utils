#include "../list.h"

link*
list_find_delete(list *l, int (*pred)(void*)) {
  link *p;

  /* Loop through all links until we find the pointer */
  list_foreach(l, p) {
    if(pred(&p[1])) {
      /* If there is a prev link, update its next-
         reference, otherwise update the head */
      if(p->prev)
        p->prev->next = p->next;
      else
        l->head = p->next;

      /* If there is a next link, update its prev-
         reference otherwise update the tail */
      if(p->next)
        p->next->prev = p->prev;
      else
        l->tail = p->prev;

      /* Zero references on this link */
      p->next = NULL;
      p->prev = NULL;
      
      return p;
    }
  }

  return NULL;
}

