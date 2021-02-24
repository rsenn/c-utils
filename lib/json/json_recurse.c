#include "../json_internal.h"
#include "../slist.h"

static void
json_recurse_val(jsonval* val, void (*fn)(), void* arg, int depth) {
  fn(val, arg, depth);
  switch(val->type) {
    case JSON_UNDEFINED:
    case JSON_STRING:
    case JSON_DOUBLE:
    case JSON_BOOL:
    case JSON_INT: break;

    case JSON_OBJECT: {
      if(!MAP_ISNULL(val->dictv)) {
        MAP_ITER_T t;
        MAP_FOREACH(val->dictv, t) {
          json_recurse_val(MAP_ITER_VALUE(t), fn, arg, depth + 1);
        }
      }
      break;
    }
    case JSON_ARRAY: {
      slink* link;
      for(link = val->listv; link; link = link->next) {
        json_recurse_val((jsonval*)&(link)[1], fn, arg, depth + 1);
      }
      break;
    }
  }
}

void
json_recurse(jsonval* val, void (*fn)(), void* arg) {
  json_recurse_val(val, fn, arg, 0);
}
