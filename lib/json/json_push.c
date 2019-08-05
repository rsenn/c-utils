#include "../slist.h"
#include "../json.h"

jsonval*
json_push(jsonval* arr, jsonval item) {
  if(arr->type == JSON_ARRAY) {
    slink* new = slink_new(jsonval);
    jsonval* val = slist_data(new);
    slist_push(&arr->listv, new);
    *val = item;
    return val;
  }
  return 0;
}

