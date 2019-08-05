#include "../slist.h"
#include <stdlib.h>

slink**
slist_find(slink** l, int (*pred)(slink*)) {
  while(*l) {
    if(pred(*l)) return l;
  }
  return NULL;
}

