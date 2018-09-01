#include <stdlib.h>
#include "../slist.h"

void
slist_move_head(slist *from, slist *to) {
  slink** ptr = &from->root;

  while(*ptr) 
    ptr = &(*ptr)->next;

  *ptr = to->root;
  to->root = from->root;
  from->root = NULL;
}
