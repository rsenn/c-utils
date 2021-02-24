#include "../json_internal.h"
#include "../alloc.h"

jsonval*
json_push(jsonval* arr) {
  if(arr->type == JSON_ARRAY) {
    jsonitem* item = alloc_zero(sizeof(jsonitem));
//    jsonval* val = &item->value;
    slist_push(&arr->listv, item);
    return &item->value;
  }
  return 0;
}
