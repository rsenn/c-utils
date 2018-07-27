#include <stdlib.h>
#include "../list.h"

link*
list_find(list *l, int (*pred)(void *)) {
  link *p;

  /* Loop through all links until we find the pointer */
  list_foreach(l, p) {
    if(pred(&p[1]))
      return p;
  }

  /* Not found :( */
  return NULL;
}

