#include "../json_internal.h"
#include "../alloc.h"

jsonval*
json_array_push(jsonval* val, const jsonval v) {
  if(val->type == JSON_ARRAY) {
    jsonitem* item;

    if((item = json_append(&val->itemv, v)))
      return &item->value;
  }
  return 0;
}
