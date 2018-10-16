#include <stdlib.h>
#include "../slist.h"

slink*
slist_shift(slink **link) {
  slink* next = (*link)->next;
  *link = next;
  (*link)->next = NULL;
  return *link;
}

