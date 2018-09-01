#include <stdlib.h>
#include "../slist.h"

slink*
slist_remove(slist *l, slink **p) {
  slink* ret = *p;
  *p = (*p)->next;
  ret->next = NULL;
  return ret;
}
