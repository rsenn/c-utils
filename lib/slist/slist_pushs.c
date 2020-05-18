#include "../slist.h"
#include "../str.h"
#include "../alloc.h"

#include <stdlib.h>

int
slist_pushs(slink** list, const char* s) {
  slink** ptr = list;
  slink* n;

  if((n = (slink*)alloc(sizeof(slink*) + sizeof(char*))) == NULL)
    return 0;

  while(*ptr) ptr = &(*ptr)->next;

  *ptr = n;

  n[0].next = NULL;
  *(char**)slist_data(n) = str_dup(s);
  //  *((const char**)&n[1]) = str_dup(s);
  return 1;
}
