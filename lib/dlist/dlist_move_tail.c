#include <stdlib.h>
#include "../dlist.h"

void
dlist_move_tail(dlist* from, dlist* to) {

  if(to->tail == NULL) {
    /* Copy to to-dlist */
    to->head = from->head;
    to->tail = from->tail;

  } else if(from->head != NULL) {
    /* Append from-dlist */
    from->head->prev = to->tail;
    to->tail->next = from->head;
    to->tail = from->tail;
  }

  /* Delete from-dlist */
  from->head = NULL;
  from->tail = NULL;
}
