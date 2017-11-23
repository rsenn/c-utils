#include <stdlib.h>
#include "list.h"
void
list_push(list *l, link *link) {
  /* We add to the list tail, so there's no next link */
  link->next = NULL;

  /* Previous link is the old tail */
  link->prev = l->tail;

  /* If there already is a link at the tail update
     its prev-reference, else update the head */
  if(l->tail)
    l->tail->next = link;
  else
    l->head = link;

  /* Now put the link to list tail */
  l->tail = link;
}
