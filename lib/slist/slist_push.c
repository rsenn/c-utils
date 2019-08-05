#include "../slist.h"

void
slist_push(slink** list, slink* link) {
  slink** ptr = list;

  while(*ptr)
    ptr = &(*ptr)->next;

  *ptr = link;
 }

