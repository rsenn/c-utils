#include <stdlib.h>
#include "../slist.h"

slink*
slist_unshift(slist *l) {
  slink* ret = l->root;
  l->root = ret->next;
  ret->next = NULL;
  return ret;
}

