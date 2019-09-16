#include <stdlib.h>
#include "../slist.h"

slink*
slist_shift(slink** list) {
  slink* link = *list;
  *list = link->next;
  link->next = NULL;
  return link;
}
