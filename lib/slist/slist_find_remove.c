#include "../slist.h"

slink*
slist_find_delete(slink** l, int (*pred)(slink*)) {
  slink* ret;

  while(*l && !pred(*l))
    l = &(*l)->next;

  ret = *l;
  *l = ret->next;
  ret->next = NULL;
  return ret;
}

