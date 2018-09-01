#include "../slist.h"

int
slist_pushs(slist *l, const char* s) {
  slink** ptr = &l->root;
  slink* n;
  
  if((n = malloc(sizeof(slink)+sizeof(s))) == NULL)
    return 0;

  while(*ptr)
    ptr = &(*ptr)->next;

  *ptr = n;
  
  n[0].next = NULL;
  *(const char**)(&n[1]) = s;
  return 1;
}
