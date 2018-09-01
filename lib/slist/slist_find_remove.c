#include "../slist.h"

slink*
slist_find_delete(slist *l, int (*pred)(void*)) {
  slink** ptr = &l->root;
  slink* ret;

  while(*ptr && !pred(*ptr))
    ptr = &(*ptr)->next;

  ret = *ptr;
  *ptr = ret->next;
  ret->next = NULL;
  return ret;
}

