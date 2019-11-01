#include <stdlib.h>
#include "../slist.h"

int
slist_shifts(slink** list) {
  slink* link = *list;

  *list = link->next;

  free(*(char**)(&link[1]));
  free(link);
  return 0;
}
