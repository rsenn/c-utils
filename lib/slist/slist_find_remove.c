#include "../slist.h"

/* Takes the first link for which pred is true out of the list and returns it
 * (its next pointer is cleared), or returns NULL and leaves the list alone if
 * there is none. */
slink*
slist_find_delete(slink** l, int (*pred)(slink*)) {
  slink* ret;

  while(*l && !pred(*l))
    l = &(*l)->next;

  if((ret = *l) == NULL)
    return NULL;

  *l = ret->next;
  ret->next = NULL;
  return ret;
}
