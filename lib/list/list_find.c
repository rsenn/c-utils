#include <stdlib.h>
#include "../list.h"

node*
list_find(list* l, int (*pred)(void*)) {
  node* p;

  /* Loop through all links until we find the pointer */
  list_foreach(l, p) {
    if(pred(&p[1]))
      return p;
  }

  /* Not found :( */
  return NULL;
}
