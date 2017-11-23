#include <stdlib.h>
#include "list.h"

void
list_move_head(list *from, list *to) {
  /* Nothing in to-list */
  if(to->head == NULL) {
    /* Copy to to-list */
    to->head = from->head;
    to->tail = from->tail;
  }
  /* Add lists */
  else if(from->tail != NULL) {
    /* Prepend from-list */
    from->tail->next = to->head;
    to->head->prev = from->tail;
    to->head = from->head;

    /* Delete from-list */
    from->head = NULL;
    from->tail = NULL;
  }

  /* Delete from-list */
  from->head = NULL;
  from->tail = NULL;
}

