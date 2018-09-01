#include <stdlib.h>
#include "../slist.h"

slink*
slist_remove(slist *l, slink *p) {
  slink** ptr = &l->root;

  while(*ptr) {
    if(*ptr == p) {
      *ptr = p->next;
      p->next = NULL;
      return p;
    }
    ptr = &(*ptr)->next;
  }
}

