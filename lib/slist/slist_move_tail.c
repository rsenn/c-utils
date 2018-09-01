#include <stdlib.h>
#include "../slist.h"

void
slist_move_tail(slist* from, slist* to) {
  slink** ptr = &to->root;

  while(*ptr)
    ptr = &(*ptr)->next;

  *ptr = from->root;
  from->root = NULL;
}

