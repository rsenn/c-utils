#include "../json_internal.h"
#include "../alloc.h"
#include "../byte.h"

jsonitem*
json_append(jsonitem** list, const jsonval value) {
  jsonitem* item;
  if((item = (jsonitem*)slink_new(jsonval))) {
    byte_copy(&item->value, sizeof(jsonval), &value);
    while(*list)
      list = &((*list)->next);
    item->next = *list;
    *list = item;
  }
  return item;
}
