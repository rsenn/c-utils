#include <stdlib.h>
#include "../dlist.h"

void
dlist_move_head(dlist* from, dlist* to) {
  /* Nothing in to-dlist */

  if(to->head == NULL) {
    /* Copy to to-dlist */
    to->head = from->head;
    to->tail = from->tail;
  }

  /* Add lists */
  else if(from->tail != NULL) {
    /* Prepend from-dlist */
    from->tail->next = to->head;
    to->head->prev = from->tail;
    to->head = from->head;

    /* Delete from-dlist */
    from->head = NULL;
    from->tail = NULL;
  }

  /* Delete from-dlist */
  from->head = NULL;
  from->tail = NULL;
}
