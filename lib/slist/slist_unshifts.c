#include "../slist.h"

int
slist_unshifts(slink** list, const char* s) {
  slink* n;
  
  if((n = malloc(sizeof(slink*)+sizeof(char*))) == NULL)
    return 0;

  n->next = *list;
  *list = n;

  *(const char**)(&n[1]) = str_dup(s);
  return 1;
}
