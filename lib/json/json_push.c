#include "../json_internal.h"
#include "../alloc.h"

jsonval*
json_push(jsonval* arr, const jsonval v) {
  if(arr->type == JSON_ARRAY) {
    jsonitem* item;

    if((item = json_append(&arr->itemv, v)))
      return &item->value;
  }
  return 0;
}
