#include <stdlib.h>
#include "list.h"

void
list_remove(list *l, link *link) {
  /* If there is a prev link, update its next-
     reference, otherwise update the head */
  if(link == l->head)
    l->head = link->next;
  else
    link->prev->next = link->next;

  /* If there is a next link, update its prev-
     reference otherwise update the tail */
  if(l->tail == link)
    l->tail = link->prev;
  else
    link->next->prev = link->prev;

  /* Zero references on this link */
  link->next = NULL;
  link->prev = NULL;
}

