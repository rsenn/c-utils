#include <stdlib.h>
#include "../slist.h"

slink*
slist_remove(slink **p) {
  slink* ret = *p;
  *p = ret->next;
  ret->next = NULL;
  return ret;
}
