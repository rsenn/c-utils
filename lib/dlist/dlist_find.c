#include <stdlib.h>
#include "../dlist.h"

node*
dlist_find(dlist* l, int (*pred)(void*)) {
  node* p;

  /* Loop through all links until we find the pointer */
  dlist_foreach(l, p) {
    if(pred(&p[1]))
      return p;
  }

  /* Not found :( */
  return NULL;
}
