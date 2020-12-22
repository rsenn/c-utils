#include <stdlib.h>
#include "../list.h"

void
dlist_move_tail(list* from, list* to) {

  if(to->tail == NULL) {
    /* Copy to to-list */
    to->head = from->head;
    to->tail = from->tail;

  } else if(from->head != NULL) {
    /* Append from-list */
    from->head->prev = to->tail;
    to->tail->next = from->head;
    to->tail = from->tail;
  }

  /* Delete from-list */
  from->head = NULL;
  from->tail = NULL;
}
