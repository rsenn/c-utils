#include "../slist.h"

slink**
slist_push(slink** list, void* arg) {
  slink* link = arg;
  while(*list) list = &(*list)->next;

  *list = link;

  return &link->next;
}
