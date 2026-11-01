#include "../slist.h"

/* Returns the pointer that points to the first link for which pred is true,
 * so that the link can be removed or replaced through it, or NULL if there is
 * none. */
slink**
slist_find(slink** l, int (*pred)(slink*)) {
  while(*l) {
    if(pred(*l))
      return l;

    l = &(*l)->next;
  }
  return NULL;
}
