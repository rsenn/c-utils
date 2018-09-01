#include <stdlib.h>
#include "../slist.h"

slink**
slist_find(slist *l, int (*pred)(void *)) {
  slink **p;

  /* Loop through all links until we find the pointer */
  slist_foreach(l, p) {
    if(pred(&(*p)[1]))
      return p;
  }

  /* Not found :( */
  return NULL;
}

