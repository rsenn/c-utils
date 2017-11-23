#include <stdlib.h>
#include "list.h"
void
list_unshift(list *l, link *link)
{
  /* We add to the list head, so there's no previous link */
  link->prev = NULL;

  /* Next link is the old head */
  link->next = l->head;

  /* If there already is a link at the head update
     its prev-reference, else update the tail */
  if(l->head)
    l->head->prev = link;
  else
    l->tail = link;

  /* Now put the link to list head */
  l->head = link;
}

